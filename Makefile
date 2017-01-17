recall_precision:main.o singly_linked_list_node.o
	cc -o recall_precision main.o singly_linked_list_node.o -lm
main.o:main.c
	cc -c main.c
singly_linked_list_node.o:singly_linked_list_node.c singly_linked_list_node.h
	cc -c singly_linked_list_node.c singly_linked_list_node.h
.PHONY:clean
clean:
	-rm recall_precision main.o singly_linked_list_node.o singly_linked_list_node.h.gch
Debug:
	cc -o recall_precision main.c singly_linked_list_node.c singly_linked_list_node.h -lm -g
