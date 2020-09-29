default:
	gcc cat.c -o cat
	gcc rm.c -o rm
	gcc OS_2.c && ./a.out
clear:
	rm -rf ./a.out history.txt cat rm