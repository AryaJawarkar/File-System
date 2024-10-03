#include"ecdc.h"

result Decoder(unsigned char *outarr){
	int i_counter = 0,break_the_loop = 1;
	unsigned char indicator = 0;
	int readBits = 8;
	unsigned char ele = 0;
	int decodeArraySize = 1;
	int i_decode_count = 0;
	result deco_info; 
	deco_info.decoded_array = (unsigned char*)malloc(decodeArraySize*sizeof(unsigned char));
	if(!deco_info.decoded_array){
		perror("space not allocated");
	}
	while(break_the_loop){
    	if(readBits == 8){
    		if((*(outarr+(i_counter/8)) & mask(i_counter%8)) == 0){
    			indicator = 0;
    		}
                i_counter += 1;
    		if((*(outarr+(i_counter/8)) & mask(i_counter%8)) != 0){
    			indicator = 1;
    		}
                i_counter += 1;
    		if(indicator == 1){
    			readBits = 0;
    		}
    		else{   
			deco_info.no_of_bytes_read = ceil(((float)(i_counter))/((float)(8)));
    	 		break_the_loop = 0;
    		}
    	} else{
    		ele <<= 1;
    		ele |= (((*(outarr+(i_counter/8)) & mask(i_counter%8)) != 0)?1:0);
    		readBits += 1;
            if(readBits == 8){
                deco_info.decoded_array[i_decode_count] = ele;
                i_decode_count += 1;
		deco_info.decoded_array = (unsigned char*)realloc(deco_info.decoded_array,((decodeArraySize + 1)));
		decodeArraySize+=1;
		if(!deco_info.decoded_array){
			perror("space not allocated");
		}
            }
            i_counter += 1;
    	}
	}    
        return deco_info;
}
