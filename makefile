OBJECT = aStar  List.o status.o ListTest.o

aStar : $(OBJECT)
	cc -o $(OBJECT)
depend:
	sed '/^#DEP/q' makefile> mk.tmp
	cc -M *.c >> mk.tmp
	mv mk.tmp makefile

#DEPENDENCIES : don't delete this line !!!