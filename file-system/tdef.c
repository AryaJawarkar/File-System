#include"ecdc.h"


int main(int argc,char *argv[]){

	FILE *fdvd = fopen(argv[1],"wb+");
	
	if(!fdvd)
		return EXIT_FAILURE;

	int del_file_size = 0,flag = 1,del_pointer = 0,bytesToRead = 0,break_the_loop = 1,cur_meta_size = 0;
	char buffer[BUFFER_SIZE];
	char isDeleted[2];

	while(break_the_loop){
	        fread(buffer, 1, sizeof(buffer), fdvd);
		if(buffer[0] == 'y' || buffer[0] == 'n'){
			isDeleted[0] = buffer[0];
			printf("%c\n",isDeleted[0]);
			isDeleted[1] = '\0';
		}
		if(strcmp(isDeleted,"n") == 0 && del_file_size == 0){
			//continue traversing
	  		if (buffer[0] == '\0') {
			       break_the_loop = 0;
	        	}	
			else {
	            		bytesToRead += 1;
	            		fseek(fdvd, bytesToRead, SEEK_SET);
 	           		memset(buffer, 0, BUFFER_SIZE);
	            		fread(buffer, 1, sizeof(buffer), fdvd);

            			// Decode metadata
	       		     	result res1 = Decoder(buffer);
 	           		bytesToRead += res1.no_of_bytes_read;

            			// Read file start position
  	          		memset(buffer, 0, BUFFER_SIZE);
	            		fread(buffer, 1, sizeof(buffer), fdvd);
	            		result res2 = Decoder(buffer);
 	           		bytesToRead += res2.no_of_bytes_read;
  	          		int file_start = *(int*)(res2.decoded_array);

            			// Read file length
	            		memset(buffer, 0, BUFFER_SIZE);
	            		fread(buffer, 1, sizeof(buffer), fdvd);
	            		result res3 = Decoder(buffer);
	            		bytesToRead += res3.no_of_bytes_read;
	            		int file_length = *(int*)(res3.decoded_array);

		    		fseek(fdvd, bytesToRead, SEEK_SET);
	            		memset(buffer, 0, BUFFER_SIZE);
			}
		}
		else if(strcmp(isDeleted,"y") == 0){
			/* 
			 * if(flag) 
			 * set pointer to pos where data is to be copied
			 * !flag
			 */
			if(flag){
				del_pointer = bytesToRead;
				flag = 0;
			}
	  		if (buffer[0] == '\0') {
			       break_the_loop = 0;
	        	}	
			else{
				bytesToRead += 1;
                                fseek(fdvd, bytesToRead, SEEK_SET);
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);

            			// Decode metadata
                                result res1 = Decoder(buffer);
                                bytesToRead += res1.no_of_bytes_read;

            			// Read file start position
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);
                                result res2 = Decoder(buffer);
                                bytesToRead += res2.no_of_bytes_read;
                                int file_start = *(int*)(res2.decoded_array);

            			// Read file length
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);
                                result res3 = Decoder(buffer);
                                bytesToRead += res3.no_of_bytes_read;
                                int file_length = *(int*)(res3.decoded_array);
				del_file_size += bytesToRead;

                                fseek(fdvd, bytesToRead, SEEK_SET);
                                memset(buffer, 0, BUFFER_SIZE);
			}
			//delsize increased
			//calc size of metadata for which shifting is to be done
			//write zeroes in it
		}
		else if(strcmp(isDeleted,"n") == 0 && del_file_size){
			// do the shifting
			// calculate meta size ajust file pointer read the data 
			// and paste it on the point from where i got del pointer
	  		/*if (buffer[0] == '\0') {
		       		break_the_loop = 0;
	       		}	
			else{        
				bytesToRead += 1;
                                fseek(fdvd, bytesToRead, SEEK_SET);
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);

                                // Decode metadata
                                result res1 = Decoder(buffer);
                                bytesToRead += res1.no_of_bytes_read;

                                // Read file start position
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);
                                result res2 = Decoder(buffer);
                                bytesToRead += res2.no_of_bytes_read;
                                int file_start = *(int*)(res2.decoded_array);

                                // Read file length
                                memset(buffer, 0, BUFFER_SIZE);
                                fread(buffer, 1, sizeof(buffer), fdvd);
                                result res3 = Decoder(buffer);
                                bytesToRead += res3.no_of_bytes_read;
                                int file_length = *(int*)(res3.decoded_array);
                                //del_file_size += bytesToRead;

				cur_meta_size = res1.no_of_bytes_read + res2.no_of_bytes_read + res3.no_of_bytes_read;
                                fseek(fdvd, bytesToRead, SEEK_SET);
                                memset(buffer, 0, BUFFER_SIZE);
			}
			int meta_to_copy = del_pointer + del_file_size;
			printf("%d\n",meta_to_copy);
			for (long src_pos = meta_to_copy, dest_pos = del_pointer; src_pos <= cur_meta_size; src_pos++, dest_pos++) {
 		       		// Read a single character from the source position
        			fseek(fdvd, src_pos, SEEK_SET);
			        char ch;
        			fread(&ch, 1, 1, fdvd);

        			// Write the character to the destination position
        			fseek(fdvd, dest_pos, SEEK_SET);
        			fwrite(&ch, 1, 1, fdvd);
    			}
			del_pointer += cur_meta_size;
		*/
		}
	}
}
