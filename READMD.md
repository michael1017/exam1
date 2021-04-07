# exam1
## Intro
NTHUEE240500 exam1.
## Installation
* prerequirement
	* python: version >= 3.6
	* git: version >= 2.17
	* 107060005.csv
	* latest pip
	    * matplotlib
```
git clone https://github.com/michael1017/hw2.git
```

## Execution
Excute the command below, the compiled binary will be sent to your STM32 device.
```
sudo mbed compile --source . --source ~/ee2405/mbed-os-build/ -m B_L4S5I_IOT01A -t GCC_ARM -f
``` 
After that follow the wire connection discribe in main.cpp
![](https://i.imgur.com/y8khUTG.png)
![](https://i.imgur.com/Lo8KuDG.jpg)

FTT.py is a python script to analysis the output signal with using matplotlib

## Example Output

### 1
![](https://i.imgur.com/M3UhYBi.png)
![](https://i.imgur.com/ez0PA6p.png)


### 1/2
![](https://i.imgur.com/DtKOVvL.png)
![](https://i.imgur.com/jEkc4Ok.png)


### 1/4
![](https://i.imgur.com/P2fpbGE.png)
![](https://i.imgur.com/ZYQCNjm.png)


### 1/8
![](https://i.imgur.com/ZTBqTJF.png)
![](https://i.imgur.com/NZcIjHW.png)



