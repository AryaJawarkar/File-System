#include "ecdc.h"


int main(int argc, char *argv[]) {
    FILE *fdvd = fopen(argv[1], "rb+"); // Changed to "rb+" for reading and writing in binary mode
    if (!fdvd)
        return EXIT_FAILURE;

    int del_file_size = 0, flag = 1, del_pointer = 0, bytesToRead = 0, break_the_loop = 1, cur_meta_size = 0;
    char buffer[BUFFER_SIZE];
    char isDeleted;

    while (break_the_loop) {
        // Reading one character to check if the file is deleted
        fread(&isDeleted, 1, 1, fdvd);
	printf("isDeleted: %c, del_file_size: %d\n", isDeleted, del_file_size);

        // If isDeleted is null character, end the loop
        if (isDeleted == '\0') {
            break_the_loop = 0;
            break;
        }

	bytesToRead += 1;
        if (isDeleted == 'n' && del_file_size == 0) {
            // Continue traversing non-deleted file metadata
            printf("Non-deleted file found.\n");

            // Read metadata
            memset(buffer, 0, BUFFER_SIZE);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res1 = Decoder(buffer);
            bytesToRead += res1.no_of_bytes_read;

            // Read file start position
            memset(buffer, 0, BUFFER_SIZE);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res2 = Decoder(buffer);
            bytesToRead += res2.no_of_bytes_read;
            int file_start = *(int *)(res2.decoded_array);

            // Read file length
            memset(buffer, 0, BUFFER_SIZE);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res3 = Decoder(buffer);
            bytesToRead += res3.no_of_bytes_read;
            int file_length = *(int *)(res3.decoded_array);

            fseek(fdvd, bytesToRead, SEEK_SET);
            memset(buffer, 0, BUFFER_SIZE);
        } else if (isDeleted == 'n' && del_file_size) {
            // Perform the file shifting
            printf("Shifting file data...\n");

            // Calculate current metadata size
            memset(buffer, 0, BUFFER_SIZE);
		fseek(fdvd,bytesToRead,SEEK_SET);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res1 = Decoder(buffer);
            bytesToRead += res1.no_of_bytes_read;

            // Read file start position
            memset(buffer, 0, BUFFER_SIZE);
		fseek(fdvd,bytesToRead,SEEK_SET);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res2 = Decoder(buffer);
            bytesToRead += res2.no_of_bytes_read;
            int file_start = *(int *)(res2.decoded_array);

            // Read file length
            memset(buffer, 0, BUFFER_SIZE);
		fseek(fdvd,bytesToRead,SEEK_SET);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res3 = Decoder(buffer);
            bytesToRead += res3.no_of_bytes_read;
            int file_length = *(int *)(res3.decoded_array);

            cur_meta_size = res1.no_of_bytes_read + res2.no_of_bytes_read + res3.no_of_bytes_read+1;
            fseek(fdvd, bytesToRead, SEEK_SET);
            memset(buffer, 0, BUFFER_SIZE);

            // Perform byte-by-byte shifting
            int meta_to_copy = del_pointer + del_file_size;
	    int limit = cur_meta_size + meta_to_copy;
	    //printf("limit : %d\n",limit);

            printf("Shifting from %d to %d\n", meta_to_copy, del_pointer);
            for (int src_pos = meta_to_copy, dest_pos = del_pointer; src_pos < limit; src_pos++,dest_pos++) {
                unsigned char ch,chz = 0;

                // Read a single character from the source position
                fseek(fdvd, src_pos, SEEK_SET);
                fread(&ch, 1, 1, fdvd);
                fseek(fdvd, src_pos, SEEK_SET);
                fwrite(&chz, 1, 1, fdvd);

		printf("helooooooooooooo\n");
                // Write the character to the destination position
                fseek(fdvd, dest_pos, SEEK_SET);
                fwrite(&ch, 1, 1, fdvd);
            }

            del_pointer += cur_meta_size;
        } else if (isDeleted == 'y') {
            // Handle deleted file
            if (flag) {
                printf("Deleted file found at position: %d\n", bytesToRead-1);
                del_pointer = bytesToRead-1;
                flag = 0;
            }
            // Read metadata of deleted file
	    //fseek(fdvd,bytesToRead-1,SEEK_SET);
            memset(buffer, 0, BUFFER_SIZE);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res1 = Decoder(buffer);
            bytesToRead += res1.no_of_bytes_read;
	    printf("%s\n ",res1.decoded_array);

            // Read file start position
            memset(buffer, 0, BUFFER_SIZE);
	    fseek(fdvd,bytesToRead,SEEK_SET);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res2 = Decoder(buffer);
            bytesToRead += res2.no_of_bytes_read;
            int file_start = *(int *)(res2.decoded_array);
	    printf("%d\n ",(*(int*)(res2.decoded_array)));

            // Read file length
            memset(buffer, 0, BUFFER_SIZE);
	    fseek(fdvd,bytesToRead,SEEK_SET);
            fread(buffer, 1, sizeof(buffer), fdvd);
            result res3 = Decoder(buffer);
            bytesToRead += res3.no_of_bytes_read;
            int file_length = *((int *)(res3.decoded_array));
	    printf("%d\n ",file_length);

            del_file_size = res1.no_of_bytes_read+res2.no_of_bytes_read+res3.no_of_bytes_read+1;

            fseek(fdvd, bytesToRead, SEEK_SET);
            memset(buffer, 0, BUFFER_SIZE);
	    printf("del_file_size : %d\n",del_file_size);
        }
    }

    fclose(fdvd);
    return 0;
}

