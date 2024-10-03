#include "ecdc.h"

int main(int argc,char* argv[]){
	FILE *fdvd,*fdfile;
	
	fdfile = fopen(argv[1],"rb+");

	fdvd = fopen((const char *)argv[3],"rb+");

	if (!fdfile || !fdvd)
		return 0;

	unsigned char buffer[BUFFER_SIZE];
	int bytesToRead = 0;

	int meta_size = 0,remaining_size = 0;
	fread(buffer,1,sizeof(buffer),fdvd);
	int break_the_loop = 1;
	fseek(fdvd,0,SEEK_END);
	int last_file_start = ftell(fdvd);
	int total_vdSize = ftell(fdvd);
	fseek(fdvd,0,SEEK_SET);
	int total_file_size = 0;
 	char isDeleted;

	while(break_the_loop){
		isDeleted = buffer[0];
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
			total_file_size += *(int *)(res3.decoded_array);
			printf("Total file size : %d\n",total_file_size);
			fseek(fdvd,bytesToRead,SEEK_SET);
			memset(buffer,0,BLOCK_SIZE);
			fread(buffer,1,sizeof(buffer),fdvd);
			fseek(fdvd,bytesToRead,SEEK_SET);
			if(bytesToRead == (total_vdSize - total_file_size)){
				return 0;
			}
		}
	}

		char del[1] = "n";

		result inp1 = Encoder(argv[1],strlen(argv[1]));
	
		int check_last_file_start = last_file_start - atoi(argv[2]);
		result inp2 = Encoder((char *)(&(check_last_file_start)),4);
		
		int file_len = atoi(argv[2]);
		result inp3 = Encoder((char *)(&(file_len)),4);
		
		int curr_meta_size = 1 + inp1.no_of_bytes_read + inp2.no_of_bytes_read + inp3.no_of_bytes_read;

	if((last_file_start-bytesToRead) >= (curr_meta_size + atoi(argv[2]))){
		fwrite(del,1,1,fdvd);
		fwrite(inp1.decoded_array,1,inp1.no_of_bytes_read,fdvd);
		last_file_start = last_file_start - atoi(argv[2]);
		inp2 = Encoder((char *)(&(last_file_start)),4);
		fwrite(inp2.decoded_array,1,inp2.no_of_bytes_read,fdvd);
	
		fwrite(inp3.decoded_array,1,inp3.no_of_bytes_read,fdvd);
		fseek(fdvd,last_file_start,SEEK_SET);
		unsigned char ch;
		while((fread(&ch,1,1,fdfile))){fputc(ch,fdvd);}
		remaining_size = total_vdSize - (bytesToRead + curr_meta_size + total_file_size + atoi(argv[2]));
		printf("Remaining Size : %d\n",remaining_size);
		
	}
	else{
		printf("no available space\n");
		return EXIT_FAILURE;
	}

	fclose(fdvd);
	fclose(fdfile);
return 0;
}
