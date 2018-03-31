CC := gcc
OBJ := main.o customer.o menu.o ticketlist.o
main:$(OBJ)
	$(CC) $^ -o $@
%.o:%.c
	$(CC) $^ -o $@ -c
clean:
	rm *.o -fr
