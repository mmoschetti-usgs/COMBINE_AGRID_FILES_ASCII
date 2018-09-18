#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFLEN 300
#define NBIG 500000
#define MAXCHAR 128
#define NP 100
#define NTOT 10
#define STREQ(s1,s2) (strcmp(s1,s2) == 0)

typedef struct header {
   char name[MAXCHAR][6];
   float period;
   int nlev;
   float xlev[20];
   float extra[10];
} HEADER;

// fortran functions
//int lxyinint_(float *x, float *y, float px[], float py[], int *n);
int lxyint_(float *x, float *y, float *px, float *py, int *n);
int kpscr_(float *x1, float *y1, float *x2, float *y2);
int read_polygon_files(char *fname, float lon[], float lat[], int *nval);

/*--------------------------------------------------------------------------*/
int get_params(char* parfile, float* latmin, float* latmax, float* lonmin, 
	float* lonmax, float* dx, float* dy, int* hdr_log)
/*--------------------------------------------------------------------------*/
{
  FILE *fppar;
  char buff[BUFFLEN];

  if ( STREQ("--help",parfile) ) {
    fprintf(stdout,"Parameter file format:\nlat-min lat-max\nlon-min lon-max\ndx dy\nheader logical(1=yes; 0=no)\n");
    return 0;
  }

// open parameter file for reading
  if((fppar = fopen(parfile, "r"))==NULL) {
    return(0);
  }
  fgets(buff,BUFFLEN,fppar);
  sscanf(buff,"%f %f", latmin, latmax);
  fgets(buff,BUFFLEN,fppar);
  sscanf(buff,"%f %f", lonmin, lonmax);
  fgets(buff,BUFFLEN,fppar);
  sscanf(buff,"%f %f", dx, dy);
  fgets(buff,BUFFLEN,fppar);
  sscanf(buff,"%d", hdr_log);
  fclose(fppar);
  
  return 1;
}


/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[])
/*--------------------------------------------------------------------------*/
{
  FILE *fpin, *fpout, *fp_agridlist;
  int hdr_log, nx, ny, nval, ndata, readn, cnt;
  float latmin, latmax, lonmin, lonmax, dx, dy;
  float agrid[NBIG], agrid_combine[NBIG];
  char filein[200], fileout[200], parfile[200], agridf[200];
  char buff[BUFFLEN], listf[200];

/* CHECK INPUT ARGUMENTS */
  if( argc!=4 ) {
    fprintf(stderr,"USAGE: %s [list of agrid files (binary)] [outfile(binary)] [parameter file]\n", argv[0]);
    fprintf(stderr,"For parameter file syntax enter --help for parameter file.\n");
    exit(1);
  }
  sscanf(argv[1],"%s", listf);
  sscanf(argv[2],"%s", fileout);
  sscanf(argv[3],"%s", parfile);

//initialize
  for (cnt=0; cnt<NBIG; cnt++) agrid_combine[cnt]=0.0;

// parameter file reading
  if ( ! get_params(parfile, &latmin, &latmax, &lonmin, &lonmax, &dx, &dy, &hdr_log) ) {
    fprintf(stderr,"Error reading parameter file, %s\n", parfile);
    exit(1);
  }
  nx=(int)((lonmax-lonmin)/dx)+1;
  ny=(int)((latmax-latmin)/dy)+1;
  nval=nx*ny;
  if (nval > NBIG){
    fprintf(stdout,"nval>NBIG: %d %d\n", nval, (int)NBIG);
    fprintf(stdout,"Increase array size.\n");
    exit(1);
  }

// loop through the list of agrid files, adding to a cumulative agrid file
  if((fp_agridlist = fopen(listf, "r"))==NULL) {
    fprintf(stderr,"Cannot open list of agrid files, %s\n", listf);
    exit(1);
  }
  while(fgets(buff,BUFFLEN,fp_agridlist) ) {
    sscanf(buff,"%s", agridf);
    if((fpin = fopen(agridf, "rb"))==NULL) {
      fprintf(stderr,"Cannot open input file - %s\n", filein);
      exit(1);
    }
// read header, if required
    if(hdr_log==1) {
      ndata=308; 		// number of bytes
//    readn=fread(&headr,ndata,1,fpin);
//    fread(headr,ndata,1,fpin);
//    fread(headr,sizeof(HEADER),1,fpin);
    }
// read data and combine with others
    readn=fread(agrid,sizeof(float),nval,fpin);
    fclose(fpin);
    for (cnt=0; cnt<nval; cnt++) agrid_combine[cnt]+=agrid[cnt];
  }

// write results to file
  fpout = fopen(fileout, "wb");
  fwrite(agrid_combine,sizeof(float),nval,fpin);
  fclose(fpout);
  fprintf(stderr,"Combined agrids in file, %s\n", fileout);

  return 0;
}
