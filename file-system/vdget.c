#include "ecdc.h"

int main(int argc,char *argv[]){
	
	if(argc != 3){
		printf("less arguements");
		return EXIT_FAILURE;
	}

	FILE *fdvd,*file_to_get;

        fdvd = fopen((const char *)argv[2],"rb+");

        if (!fdvd)
                return EXIT_FAILURE;

        unsigned char buffer[BUFFER_SIZE];
        int bytesToRead = 0;
        fread(buffer,1,sizeof(buffer),fdvd);
        int meta_size = 0;
        int break_the_loop = 1;
        fseek(fdvd,0,SEEK_END);
        int last_file_start = ftell(fdvd);
        int total_vdSize = ftell(fdvd);
        fseek(fdvd,0,SEEK_SET);
        int total_file_size = 0;
	int file_size = 0;
	char ch;
	char isDeleted[2];
	
	bool file_found = false;

        while(break_the_loop){
                /*
                 */
		isDeleted[0] = buffer[0];
		isDeleted[1] = '\0';
                if(buffer[0] == '\0'){
                        break_the_loop = 0;
                }
                else{
                	bytesToRead = bytesToRead + 1;
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);
			
			result res1 = Decoder(buffer);
                        bytesToRead = bytesToRead + res1.no_of_bytes_read;
                        fseek(fdvd,bytesToRead,SEEK_SET);

                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);

                        result res2 = Decoder(buffer);
                        bytesToRead += res2.no_of_bytes_read;
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        last_file_start = *(int*)(res2.decoded_array);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        result res3 = Decoder(buffer);
                        bytesToRead += res3.no_of_bytes_read;
                        file_size = *(int *)(res3.decoded_array);
                        total_file_size += *(int *)(res3.decoded_array);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        if(bytesToRead == (total_vdSize - total_file_size)){
                                return EXIT_FAILURE;
                        }
			if(strcmp(res1.decoded_array,argv[1]) == 0 && strcmp(isDeleted,"n") == 0){
				file_found = true;
				file_to_get = fopen(res1.decoded_array,"wb+");
				if(!file_to_get){
					printf("file not created\n");
				}
				fseek(fdvd,last_file_start,SEEK_SET);
				while((ch=fgetc(fdvd))!=255 && file_size >= 1){
					fputc(ch,file_to_get);
					file_size--;
				}
				printf("File added to your drive\n");
			}
                }

        }

	if(!file_found){
		printf("No such file exists\n");
	}
        fclose(fdvd);

return 0;
}
