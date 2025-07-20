################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
GrLib/grlib/%.obj: ../GrLib/grlib/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs2011/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmsp -O3 --use_hw_mpy=none --include_path="/Applications/ti/ccs2011/ccs/ccs_base/msp430/include" --include_path="/Users/grant/Library/CloudStorage/OneDrive-UniversityofStrathclyde/Masters/Advanced_Microcontrollers/C/challenge1AOCLCrossing" --include_path="/Users/grant/Library/CloudStorage/OneDrive-UniversityofStrathclyde/Masters/Advanced_Microcontrollers/C/challenge1AOCLCrossing/GrLib/grlib" --include_path="/Users/grant/Library/CloudStorage/OneDrive-UniversityofStrathclyde/Masters/Advanced_Microcontrollers/C/challenge1AOCLCrossing/GrLib/fonts" --include_path="/Applications/ti/ccs2011/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --advice:power="none" --define=__MSP430G2553__ -g --gcc --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="GrLib/grlib/$(basename $(<F)).d_raw" --obj_directory="GrLib/grlib" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


