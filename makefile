CXX = g++
OBJECTS = http_service.o pig_latin.o

http_service: http_service.o pig_latin.o
	$(CXX) -o http_service http_service.o pig_latin.o

http_service.o: http_service.cpp
	$(CXX) -c http_service.cpp pig_latin.h

pig_latin.o: pig_latin.h

clean:
	rm -f $(OBJECTS)
