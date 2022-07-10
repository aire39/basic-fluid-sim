Fluid Simulation

This is Mike Ashes work converted to be used for 2D instead of 3d and rendered using the SFML library which you can find here:

Link to author web blog: https://mikeash.com/pyblog/fluid-simulation-for-dummies.html  
Link to paper: https://www.dgp.toronto.edu/public_user/stam/reality/Research/pdf/GDC03.pdf (author Jos Stam)

This is meant to be a learning experience for myself. The plan is to use vulkan compute shaders to process the simulation on the
GPU though this may require a different approach.

compile:
you can run the buildme.sh script

Tested on: MSVC, MSYS2 (MinGW64), Linux (Manjaro)

![fluid_density](images/fluid_density_dye.png)
![fluid_density](images/fluid_velocity.png)