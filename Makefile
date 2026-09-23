all: tct nasa pdt

tct:	
	g++ -std=c++17 -Wall TimeCode.cpp TimeCodeTests.cpp -o tct

nasa:
	g++ -std=c++17 -Wall TimeCode.cpp NasaLaunchAnalysis.cpp -o nasa

pdt:
	g++ -std=c++17 -Wall TimeCode.cpp PaintDryTimer.cpp -o pdt

clean:
	rm -f tct nasa pdt
