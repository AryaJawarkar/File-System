#include"ecdc.h"

int main(int argc,char *argv[]){

	FILE *fdvd;

        fdvd = fopen((const char *)argv[1],"wb+");

        if (!fdvd)
                return EXIT_FAILURE;

        unsigned char buffer[BUFFER_SIZE];
        int bytesToRead = 0;

        int meta_size = 0;
        fread(buffer,1,sizeof(buffer),fdvd);
        int break_the_loop = 1;
        fseek(fdvd,0,SEEK_END);
        int last_file_start = 0;
        int total_vdSize = ftell(fdvd);
        fseek(fdvd,0,SEEK_SET);
        int total_file_size = 0,total_meta_size = 0;
        char isDeleted[2];
	int file_length = 0,del_file_length = 0,file_start = 0,last_file_length = 0;

        while(break_the_loop){
                isDeleted[0] = buffer[0];
		isDeleted[1] = '\0';
                if(buffer[0] == '\0'){
                        break_the_loop = 0;
                }else{

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
                        file_start = *(int*)(res2.decoded_array);
                        last_file_start = *(int*)(res2.decoded_array);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);

                        result res3 = Decoder(buffer);
                        bytesToRead += res3.no_of_bytes_read;
                        file_length = *(int *)(res3.decoded_array);
                        total_file_size += *(int *)(res3.decoded_array);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        memset(buffer,0,BLOCK_SIZE);
                        fread(buffer,1,sizeof(buffer),fdvd);
                        fseek(fdvd,bytesToRead,SEEK_SET);
                        if(bytesToRead == (total_vdSize - total_file_size)){
                                return EXIT_FAILURE;
                        }

                }
		
	/*		if(strcmp((const char *)isDeleted,"y") == 0)
			{
				del_file_length += file_length;
			}
			else if (strcmp(isDeleted,"n") == 0 && del_file_length != 0) {
		                long remaining_file_size = file_length;
		                fseek(fdvd, file_start + file_length, SEEK_SET);

   			        while (remaining_file_size > 0) {
   	                 		size_t chunk_size = (remaining_file_size < CHUNK_SIZE) ? remaining_file_size : BUFFER_SIZE;

                   			fseek(fdvd, file_start + remaining_file_size - chunk_size, SEEK_SET);
                  			fread(buffer, 1, chunk_size, fdvd);

                   			fseek(fdvd, new_file_start_pos - remaining_file_size, SEEK_SET);
                   			fwrite(buffer, 1, chunk_size, fdvd);

                   			remaining_file_size -= chunk_size;
                		}	
             		 	new_file_start_pos -= file_length;
               			last_file_start = new_file_start_pos;
            }

            fseek(fdvd, bytesToRead, SEEK_SET);
            memset(buffer, 0, CHUNK_SIZE);
            fread(buffer, 1, sizeof(buffer), fdvd);
        }

        }

             
	if (del_file_length == total_file_size) {
        	fseek(fdvd, 0, SEEK_SET);
        	unsigned char buffer[BUFFER_SIZE] = {0};

        	for (int i = 0; i < total_vdSize / BLOCK_SIZE; i++) {
            		fwrite(buffer, 1, BLOCK_SIZE, fdvd);
        	}
   	 }
*/
fclose(fdvd);
return 0;
}
