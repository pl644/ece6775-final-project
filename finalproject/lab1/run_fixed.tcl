#=============================================================================
# run-fixed.tcl 
#=============================================================================
# @brief: A Tcl script for fixed-point experiments.
#
# @desc: This script runs a batch of simulation & synthesis experiments
# to explore trade-offs between accuracy, performance, and area for 
# fixed-point implementation of the CORDIC core.
#
# 1. The user specifies a list of bitwidth pairs, i.e., (FULL_WIDTH, I_WIDTH)
# 2. Results are collected in a single file ./result/fixed-result.csv
# 3. out.dat from each individual experiment is also copied to ./result

#------------------------------------------------------
# Set result filename and clean old data
#------------------------------------------------------
set filename "fixed_result.csv"
file delete -force "./result/${filename}"

#-----------------------------------------------------
# You can specify a set of bitwidth configurations to 
# explore in the batch experiments. 
# Each configuration (line) is defined by a pair in  
# total bitwidth and integer bitwidth
# Examples are shown below. 
#-----------------------------------------------------
set bitwidth_pair { \
    32    16   \
    64    32   \
}

#-----------------------------------
# run batch experiments
#-----------------------------------
foreach { TOT_W INT_W } $bitwidth_pair {

# Define the bitwidth macros from CFLAGs
set CFLAGS "-DFIXED_TYPE -DFULL_WIDTH=${TOT_W} -DI_WIDTH=${INT_W}"

# Project name
set hls_prj "fixed_${TOT_W}_${INT_W}_20.prj"

# Open/reset the project
open_project ${hls_prj} -reset
# Top function of the design is "cordic"
set_top cordic

# Add design and testbench files
add_files cordic.cpp -cflags $CFLAGS
add_files -tb cordic_test.cpp -cflags $CFLAGS

open_solution "solution1"
# Use Zynq device
set_part {xc7z020clg484-1}

# Target clock period is 10ns
create_clock -period 10

# Simulate the C++ design
csim_design
# Synthesize the design
csynth_design

# We will skip C-RTL cosimulation for now
#cosim_design

#---------------------------------------------
# Collect & dump out results from HLS reports
#---------------------------------------------
set argv [list $filename $hls_prj]
set argc 2
source "./script/collect_result.tcl"
}

quit
