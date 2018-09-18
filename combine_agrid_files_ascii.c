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

/*--------------------------------------------------------------------------*/
int get_full_lonlat(char* parfile, float* lat_arr, float* lon_arr, float *cntLonLat)
/*--------------------------------------------------------------------------*/
{
  FILE *fppar;
  int cnt=0;
  char buff[BUFFLEN];

// open parameter file for reading
  if((fppar = fopen(parfile, "r"))==NULL) {
    return(0);
  }
  while(fgets(buff,BUFFLEN,fppar) ) {
    sscanf(buff,"%f %f", lon_arr[cnt], lat_arr[cnt]);
    cnt++;
  }
  fclose(fppar);
  *cntLonLat=cnt;
  
  return 1;
}


/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[])
/*--------------------------------------------------------------------------*/
{
  FILE *fpin, *fpout, *fp_agridlist;
//  int hdr_log, nx, ny, nval, ndata, readn, cnt;
  int cnt, cntCoords;
  float latmin, latmax, lonmin, lonmax, dx, dy;
  float agrid[NBIG], agrid_combine[NBIG];
  char filein[200], fileout[200], parfile[200], agridf[200];
  char buff[BUFFLEN], listf[200];

/* CHECK INPUT ARGUMENTS */
  if( argc!=4 ) {
    fprintf(stderr,"USAGE: %s [list of agrid files (ascii)] [outfile(ascii)] [file with list lon-lat coords]\n", argv[0]);
    fprintf(stderr,"For lon-lat reference file (lon lat)\n");
    exit(1);
  }
  sscanf(argv[1],"%s", listf);
  sscanf(argv[2],"%s", fileout);
  sscanf(argv[3],"%s", parfile);

// read file to get full list lon/lat
  if ( ! get_full_lonlat(parfile, &lat_arr, &lon_arr, &cntCoords) ) {
    fprintf(stderr,"Error reading parameter file, %s\n", parfile);
    exit(1);
  }
  for(cnt=0; cnt<cntCoords; cnt++) {
    agrid_combine[cnt]=0;
  }




  return 0;
}

