All: 
	gcc -o getpidRdtscp getpidRdtscp.c -Wall -Werror -g
	gcc -o getuidRdtscp getuidRdtscp.c -Wall -Werror -g
	gcc -o pidGettime pidGetTime.c -Wall -Werror -g
	gcc -o uidGettime uidGettime.c -Wall -Werror -g

clean: 
	rm getpidRdtscp
	rm getuidRdtscp
	rm pidGettime
	rm uidGettime
