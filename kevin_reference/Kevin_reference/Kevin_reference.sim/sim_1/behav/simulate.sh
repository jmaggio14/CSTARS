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
ExecStep $xv_path/bin/xsim kmk_star_fsm_behav -key {Behavioral:sim_1:Functional:kmk_star_fsm} -tclbatch kmk_star_fsm.tcl -log simulate.log
