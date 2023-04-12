all: sample

sample: sample.cpp MIPS_Processor.hpp
	g++ sample.cpp MIPS_Processor.hpp -o sample

5stage: 5stage.cpp MIPS_Processor.hpp
	g++ 5stage.cpp MIPS_Processor.hpp -o output2 
	make clean output2

5stage_bypass: 5stage_bypass.cpp MIPS_Processor.hpp
	g++ 5stage_bypass.cpp MIPS_Processor.hpp -o output
	make clean output

clean:
	rm sample 