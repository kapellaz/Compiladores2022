
all:
	flex jucompiler.l
	yacc -d -Wcounterexamples jucompiler.y
	cc lex.yy.c y.tab.c functions.c -o jucompiler
