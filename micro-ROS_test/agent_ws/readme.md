Command to set up the container workspace:

`docker run -it --rm --net=host -v "$(pwd)/agent_ws/src:/uros_ws/src" --privileged --device=/dev/ttyACM0:/dev/ttyACM0 micro-ros-development:latest`

Run inside the container:

`ros2 run micro_ros_agent micro_ros_agent serial -b 115200 --dev /dev/ttyACM0`