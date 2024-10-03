all: starter executables test

starter:
	gcc -c encoder.c	
	gcc -c decoder.c
	ar -rcs ecdc.a encoder.o 	
	ar -rcs ecdc.a decoder.o 

executables:
	gcc vdadd.c ecdc.a -o vdAdd -lm
	gcc vdget.c ecdc.a -o vdGet -lm
	gcc vdls.c ecdc.a -o vdls -lm
	gcc vdcre.c  -o vdCreate
	gcc vdDelete.c ecdc.a -o vdDelete -lm

test:
	./vdCreate easternDigitalVD 10000000
	./vdAdd arya 27 easternDigitalVD
	./vdls easternDigitalVD
	./vdGet wrongfile easternDigitalVD
	./vdls easternDigitalVD
	./vdAdd bkp 383 easternDigitalVD
	./vdAdd vdcre.c 327 easternDigitalVD
	./vdls easternDigitalVD
	./vdGet bkp easternDigitalVD
	./vdDelete bkp easternDigitalVD
	./vdls easternDigitalVD

defrag:
	gcc tdef1.c ecdc.a -o defrag -lm
clean:
	rm -rf *.o vdAdd vdGet vdls vdCreate vdDelete easternDigitalVD
	
