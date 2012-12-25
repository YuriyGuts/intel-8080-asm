@echo off
rem --------------------------------------------------------------------------------------------
rem  Assembles all source files in the current directory and creates a build log for each file
rem --------------------------------------------------------------------------------------------

cls
color 0a
echo Reassembling all source files in current directory...
echo.

set errorsOccurred="0"
if exist BUILD_*.ASM.LOG del BUILD_*.ASM.LOG

for %%i in (*.ASM) do (
	echo Assembling %%i
	i8080asm %%i >> BUILD_%%i.LOG

	if errorlevel 1 (
		set errorsOccurred="1"
		color 0c
		echo - Error! See 'BUILD_%%i.LOG' for details
	) else (
		echo - OK
	)
	echo.
)

echo.
if %errorsOccurred%=="1" (
	echo One or more errors occurred while reassembling the sources!
	set /p var="Press Enter to close..."
) else (
	echo Done!
)
echo.
