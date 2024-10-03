#include"ecdc.h"
int main(int argc, char **argv)
{
	if (argc==3) {
			FILE *f = fopen((const char *)argv[1], "wb+");

			/*char ch = 0;
			for (int i = 0; i < atoi(argv[2]); i++) {
				fseek(f, i * sizeof(char), SEEK_SET);
				fwrite(&ch, 1, 1, f);
			}*/
			
			fseek(f,atoi(argv[2]),SEEK_SET);
			fputc(0,f);
			fclose(f);
	}
	else printf("Invalid Number of arguments\n");
	return 0;
}
