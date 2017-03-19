# 
# Synthesis run script generated by Vivado
# 

set_msg_config -id {HDL 9-1061} -limit 100000
set_msg_config -id {HDL 9-1654} -limit 100000
create_project -in_memory -part xc7z010clg400-1

set_param project.singleFileAddWarning.threshold 0
set_param project.compositeFile.enableAutoGeneration 0
set_param synth.vivado.isSynthRun true
set_property webtalk.parent_dir /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.cache/wt [current_project]
set_property parent.project_path /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.xpr [current_project]
set_property default_lib xil_defaultlib [current_project]
set_property target_language Verilog [current_project]
set_property ip_repo_paths /home/jeffmaggio/cstars/vivado_design/create_ip [current_project]
read_verilog /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.srcs/sources_1/imports/create_ip/kmk_star_fsm.h
set_property file_type "Verilog Header" [get_files /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.srcs/sources_1/imports/create_ip/kmk_star_fsm.h]
set_property is_global_include true [get_files /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.srcs/sources_1/imports/create_ip/kmk_star_fsm.h]
read_verilog -library xil_defaultlib /home/jeffmaggio/cstars/jeffstuff/code/kevin_reference/Kevin_reference/Kevin_reference.srcs/sources_1/imports/create_ip/kmk_star_fsm.v
foreach dcp [get_files -quiet -all *.dcp] {
  set_property used_in_implementation false $dcp
}

synth_design -top kmk_star_fsm -part xc7z010clg400-1


write_checkpoint -force -noxdef kmk_star_fsm.dcp

catch { report_utilization -file kmk_star_fsm_utilization_synth.rpt -pb kmk_star_fsm_utilization_synth.pb }
