@echo off

SET GenFolder=.\build
 
if not exist %GenFolder% (
        rem 创建空目录G:\Jmeter\apache-jmeter-5.0\report\OutReport
		rem 创建非空目录，使用复制方法
		echo Create folder: %GenFolder%
		md %GenFolder%
    ) else (
        rem 目录G:\Jmeter\apache-jmeter-5.0\report\OutReport已存在，无需创建
		echo The folder exists: %GenFolder%	
    )


cd build
cmake .. -G "MinGW Makefiles"
make