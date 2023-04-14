
compile: 
	g++ -o 5stage.o 5stage.cpp MIPS_Processor.hpp -I include/
	g++ -o 5stage_bypass.o 5stage_bypass.cpp MIPS_Processor.hpp -I include/
	g++ -o 79stage.o 79stage.cpp MIPS_Processor.hpp -I include/
	g++ -o 79stage_bypass.o 79stage_bypass.cpp MIPS_Processor.hpp -I include/

run_5stage: 
	g++ -o 5stage.o 5stage.cpp MIPS_Processor.hpp -I include/
	./5stage.o input.asm

run_5stage_bypass: 
	g++ -o 5stage_bypass.o 5stage_bypass.cpp MIPS_Processor.hpp -I include/
	./5stage_bypass.o input.asm

run_79stage: 
	g++ -o 79stage.o 79stage.cpp MIPS_Processor.hpp -I include/
	./79stage.o input.asm

run_79stage_bypass: 
	g++ -o 79stage_bypass.o 79stage_bypass.cpp MIPS_Processor.hpp -I include/
	./79stage_bypass.o input.asm


clean:
	rm -f 5stage.o 5stage_bypass.o 79stage.o 79stage_bypass.o