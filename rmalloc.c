#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include "rmalloc.h" 


rm_header rm_free_list = { 0x0, 0 };
rm_header rm_used_list = { 0x0, 0 };
rm_header_ptr head;
void * rmalloc (size_t s)
{
	
	if(rm_free_list.next==0x0){
		rm_header_ptr  head = rm_free_list.next;
		head =  mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		head->size = 4096 - sizeof(rm_header_ptr);
		head -> next = NULL;
		rm_free_list.next = head;
	}
	rm_header_ptr itr;
	itr = mmap(NULL, s, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
	itr ->size = s - sizeof(rm_header_ptr);
	itr->next = head;
	rm_used_list.next = itr;	
	// head->size = head->size - s;
	//rm_free_list.next = head;
	//head = mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
		
}

void rfree (void * p) 
{
	rm_header *rm_used_list = (rm_header *) p-1;
	munmap(rm_used_list, rm_used_list->size);
	
}

void * rrealloc (void * p, size_t s) 
{
	// TODO
	return 0x0 ; // erase this 
}

void rmshrink () 
{
	// TODO
}

void rmconfig (rm_option opt) 
{
	// TODO
}


void 
rmprint () 
{
	rm_header_ptr itr;
	int i;

	printf("==================== rm_free_list ====================\n");
	for (itr = rm_free_list.next, i = 0 ; itr != 0x0 ; itr = itr->next, i++) {
		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(rm_header), (int) itr->size);

		int j ;
		char * s = ((char *) itr) + sizeof(rm_header) ;
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size) ; j++) 
			printf("%02x ", s[j]) ;
		printf("\n") ;
	}
	printf("\n") ;

	printf("==================== rm_used_list ====================\n") ;
	for (itr = rm_used_list.next, i = 0 ; itr != 0x0 ; itr = itr->next, i++) {
		printf("%3d:%p:%8d:", i, ((void *) itr) + sizeof(rm_header), (int) itr->size);

		int j;
		char * s = ((char *) itr) + sizeof(rm_header);
		for (j = 0 ; j < (itr->size >= 8 ? 8 : itr->size); j++) 
			printf("%02x ", s[j]);
		printf("\n");
	}
	printf("\n");

}
