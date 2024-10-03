#include"ecdc.h"

result Encoder(char *arr,int length){

	result enco_info;
	int total_bits_required = 0;
    	int bytes = 0,ele;
    	total_bits_required += (8*length + (2*length + 2));
	enco_info.no_of_bytes_read = ceil(((float)(total_bits_required))/((float)(8)));
	enco_info.decoded_array  = (unsigned char *)malloc(enco_info.no_of_bytes_read * sizeof(unsigned char));
	int reqBits = total_bits_required-1;
	int flag = 1;
	int idx = 0;
	int shift = 0;
	int val;
	for(int i = length-1;reqBits > 0 && i>=0;i--){
	        val = (int)arr[i];
     		if(flag == 1){
     			reqBits -= 2;
     			flag = 0;
     		}
            while(idx < 8){
                if((val & 1) == 1){
                    shift = ((reqBits % 8));
                    enco_info.decoded_array[reqBits/8] |= mask(shift);
                    reqBits -= 1;
                    idx += 1;
                }else{
                    reqBits--;
                    idx += 1;
                }
                if(idx == 8){
                    shift = ((reqBits % 8));
                    enco_info.decoded_array[reqBits/8] |= mask(shift);
                    reqBits -= 2;
                }
                val = val >> 1;
            }
            idx = 0;
	}

	return enco_info;
}

