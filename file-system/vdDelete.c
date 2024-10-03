#include"ecdc.h"

int main(int argc,char *argv[]){

      	FILE *fdvd;

        fdvd = fopen((const char *)argv[2],"rb+");

        if (!fdvd)
                return EXIT_FAILURE;

        unsigned char buffer[BUFFER_SIZE];
        int bytesToRead = 0;

        int meta_size = 0;
        fread(buffer,1,sizeof(buffer),fdvd);
        int break_the_loop = 1;
        fseek(fdvd,0,SEEK_END);
        int last_file_start = ftell(fdvd);
        int total_vdSize = ftell(fdvd);
        fseek(fdvd,0,SEEK_SET);
        int total_file_size = 0;
	char del[1] = "y"; 
	char isDeleted[2];

        while(break_the_loop){
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

			if(strcmp(argv[1],res1.decoded_array) == 0 && strcmp(isDeleted,"n") == 0){
				fseek(fdvd,(bytesToRead - 1),SEEK_SET);
				fwrite(del,1,1,fdvd);
				fseek(fdvd,bytesToRead,SEEK_SET);
			}

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
                        total_file_size += *(int *)(res3.decoded_array);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);


                        if(bytesToRead == (total_vdSize - total_file_size)){
                                return EXIT_FAILURE;
                        }
                }
        }
	

return 0;
}
