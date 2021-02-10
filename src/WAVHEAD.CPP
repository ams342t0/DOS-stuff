#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <alloc.h>


int main(int argc,char**argv)
{
	FILE *fp;
	char *d;

	fp = fopen(argv[1],"rb");

	struct wavedata_chunk{
		char 		  id[4];
		unsigned long len;
	};

	struct fmt_chunk
	{
		char			id[4];
		unsigned long	len;
		unsigned int	formattag;
		unsigned int	nchannels;
		unsigned long	nsamplespersec;
		unsigned long   navgbytespersec;
		unsigned int	nblockalign;
		unsigned int	formatspecific;
	};

	struct dchunk_head{
		char					id[4];
		struct fmt_chunk		fchunk;
		struct wavedata_chunk   wdchunk;
	};

	struct riff_head{
		char 	 			id[4];
		long int 			len;
		struct dchunk_head 	dchunk;
	};

	struct riff_head h;

	fread(&h,sizeof(struct riff_head),1,fp);

	printf("\nRIFF ID        : %c%c%c%c" ,h.id[0],h.id[1],h.id[2],h.id[3]);
	printf("\nFmt Channels   : %d" ,h.dchunk.fchunk.nchannels);
	printf("\nFmt Samp/sec   : %ld" ,h.dchunk.fchunk.nsamplespersec);
	printf("\nWave data len  : %ld" ,h.dchunk.wdchunk.len);
	printf("\nDuration       : %-8.2fs" ,((double)h.dchunk.wdchunk.len)/h.dchunk.fchunk.nsamplespersec);

	unsigned long l = h.dchunk.wdchunk.len;
	char far *buffer = (char far*)farmalloc(l);
	char tmp[64];

	while (l <= 64)
	{

		l-=64;
	}

	fclose(fp);
	return 0;
}