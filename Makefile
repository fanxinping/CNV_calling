jaccard_index:main.o singly_linked_list_node.o
	cc -o jaccard_index main.o singly_linked_list_node.o -lm
main.o:main.c
	cc -c main.c
singly_linked_list_node.o:singly_linked_list_node.c singly_linked_list_node.h
	cc -c singly_linked_list_node.c singly_linked_list_node.h
.PHONY:clean
clean:
	-rm jaccard_index main.o singly_linked_list_node.o
Debug:
	cc -o jaccard_index main.c singly_linked_list_node.c singly_linked_list_node.h -lm -g
