#!/bin/bash -f
xv_path="/opt/Xilinx/Vivado/2016.1"
ExecStep()
{
"$@"
RETVAL=$?
if [ $RETVAL -ne 0 ]
then
exit $RETVAL
fi
}
ExecStep $xv_path/bin/xelab -wto 32647fb3d37a4278900a5dcd7b714655 -m64 --debug typical --relax --mt 8 -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip --snapshot kmk_star_fsm_behav xil_defaultlib.kmk_star_fsm xil_defaultlib.glbl -log elaborate.log
