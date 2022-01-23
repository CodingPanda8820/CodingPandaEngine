cd..
cd..
xcopy .\CodingPandaEngine\Bin\*.* .\Client2D\Bin\ /s /d /y /exclude:exclude.txt
xcopy .\CodingPandaEngine\Bin\*.* .\Engine\Bin\ /s /d /y /exclude:exclude.txt

xcopy .\CodingPandaEngine\Include\*.h .\Engine\Include /s /d /y
