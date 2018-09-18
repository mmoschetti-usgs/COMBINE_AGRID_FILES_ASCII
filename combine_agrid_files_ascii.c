#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define BUFFLEN 300
#define NBIG 200000
#define MAXCHAR 128
#define NP 100
#define NTOT 10
#define STREQ(s1,s2) (strcmp(s1,s2) == 0)

/*--------------------------------------------------------------------------*/
int sum_agrid_values(float lat_arr[],float lon_arr[],float agrid_comb[],int cntCoords,float lat_in[],float lon_in[],float agrid_in[],int cntIn)
/*--------------------------------------------------------------------------*/
{
  int cnt, cnt1;
  float lon1, lat1, lon2, lat2;
  float diffLat=999, diffLon=999;
  float threshVal=0.005;

// loop through values and sum where they agree
  for(cnt=0; cnt<cntCoords; cnt++) {
    lon1=lon_arr[cnt];
    lat1=lat_arr[cnt];
    for(cnt1=0; cnt1<cntIn; cnt1++) {
      lon2=lon_in[cnt1];
      lat2=lat_in[cnt1];
      diffLon=fabs(lon1-lon2);
      diffLat=fabs(lat1-lat2);
      if ( ( diffLon<threshVal ) && (diffLat < threshVal ) ) {
        agrid_comb[cnt]+=agrid_in[cnt1];
        break;
      }
    }
    diffLat=999;
    diffLon=999;
  }

  return 1;
}

/*--------------------------------------------------------------------------*/
int read_agrid_file(char* agridFile, float lat_arr[], float lon_arr[], float agrid_arr[], int *cntF)
/*--------------------------------------------------------------------------*/
{
  FILE *fp;
  int cnt=0;
  char buff[BUFFLEN];

// open parameter file for reading
  if((fp = fopen(agridFile, "r"))==NULL) {
    return(0);
  }
  while(fgets(buff,BUFFLEN,fp) ) {
    sscanf(buff,"%f %f %f", &lon_arr[cnt], &lat_arr[cnt], &agrid_arr[cnt]);
    cnt++;
  }
  fclose(fp);

//
  *cntF=cnt;
//  fprintf(stderr,"%d %d", cnt, *cntLonLat);
  
  return 1;
}


/*--------------------------------------------------------------------------*/
int get_full_lonlat(char* parfile, float lat_arr[], float lon_arr[], int *cntLonLat)
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
    sscanf(buff,"%f %f", &lon_arr[cnt], &lat_arr[cnt]);
    cnt++;
  }
  fclose(fppar);

//
  *cntLonLat=cnt;
//  fprintf(stderr,"%d %d", cnt, *cntLonLat);
  
  return 1;
}


/*--------------------------------------------------------------------------*/
int main(int argc, char *argv[])
/*--------------------------------------------------------------------------*/
{
  FILE *fpout, *fp_agridlist;
//  int hdr_log, nx, ny, nval, ndata, readn, cnt;
  int cnt, cntIn, cntCoords;
//  float agrid[NBIG];
  float lon_arr[NBIG], lat_arr[NBIG], agrid_comb[NBIG];
  float lon_in[NBIG], lat_in[NBIG], agrid_in[NBIG];
//  float lon_arr2[NBIG], lat_arr2[NBIG], agrid_arr2[NBIG];
  char filein[200], fileout[200], parfile[200];
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
fprintf(stderr,"%s file\n", parfile);

// read file to get full list lon/lat
  if ( ! get_full_lonlat(parfile, lat_arr, lon_arr, &cntCoords) ) {
    fprintf(stderr,"Error reading parameter file, %s\n", parfile);
    exit(1);
  }
  for(cnt=0; cnt<cntCoords; cnt++) agrid_comb[cnt]=0;
  fprintf(stderr,"cnt=%d\n", cntCoords);

// read files from list
  if ((fp_agridlist = fopen(listf, "r"))==NULL) {
    fprintf(stderr,"Error opening list of agrid files, %s\n", listf);
    exit(1);
  }
  while(fgets(buff,BUFFLEN,fp_agridlist) ) {
    sscanf(buff,"%s", filein);
    read_agrid_file(filein, lat_in, lon_in, agrid_in, &cntIn);
    sum_agrid_values(lat_arr,lon_arr,agrid_comb,cntCoords,lat_in,lon_in,agrid_in,cntIn);
  }
  fclose(fp_agridlist);

// write summed values to file
  fpout = fopen(fileout, "w");
  for(cnt=0; cnt<cntCoords; cnt++) {
    fprintf(fpout,"%.2f %.2f %.4e\n", lon_arr[cnt], lat_arr[cnt], agrid_comb[cnt]);
  }
  fclose(fpout);
  fprintf(stderr,"Wrote to file, %s\n", fileout);

  return 0;
}

