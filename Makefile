#получение исполняемого файла путем компоновки объектных (пока только 1 объектный). 
compile: objectfiles
	g++ -o ./build/VolvoCANtool ./build/VolvoCANtool.o -static

	
# получение объектного модуля из исходного кода. Получается ELF-файл типа "перемещаемый". Выполнить его пока нельзя, далее нужно его передать компоновщику 

objectfiles: VolvoCANtool.cpp
	g++ -c VolvoCANtool.cpp
	
	mv VolvoCANtool.o build/VolvoCANtool.o

	
clean: 
	rm -f build/*.o
	rm -f build/VolvoCANtool


