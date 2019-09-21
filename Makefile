all: pipel pipet tcpl tcpt udpl udpt

pipel pipet tcpl tcpt udpl udpt: % : %.c
	gcc -o $@ $^

clean:
	rm -f pipel pipet tcpl tcpt udpl udpt *.txt

.PHONY: clean