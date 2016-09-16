all: laserv cli vfserv

laserv: laser.c
	gcc -g laser.c -o laserv -lnsl -lsocket -lresolv

vfserv: vfser.c
	gcc -g vfser.c -o vfserv -lnsl -lsocket -lresolv


cli: cli.c
	gcc -g cli.c -o cli -lnsl -lsocket -lresolv

clean:
	rm -f  core laserv vfserv lacli
	
