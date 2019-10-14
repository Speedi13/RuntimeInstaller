# RuntimeInstaller
Microsoft Redistributable Download &amp; Installer <br />
<br />
To avoid issues with missing runtimes this project is build without the default c run-time library! <br />
Because of that only windows-API can be used. <br />
I got inspired by this article:<br />
http://www.catch22.net/tuts/win32/reducing-executable-size

# Download
[>> DOWNLOAD <<](https://github.com/Speedi13/RuntimeInstaller/releases/download/1/VsRuntimesInstaller.zip)

# About
It is downloading and installing all ``Microsoft Visual C++ Redistributable for Visual Studio`` <br/>
All the way from ``Visual Studio 2005`` to ``Visual Studio 2019`` and the ``DirectX End-User Runtime``

![Screenshot](https://i.imgur.com/Yb6DNyN.png)
<br />
To do the downloading it is using the [URLDownloadToFileW](https://docs.microsoft.com/en-us/previous-versions/windows/internet-explorer/ie-developer/platform-apis/ms775123(v%3Dvs.85)) API<br />
You can find the URL-list here:<br />
[RedistLibrary.h](https://github.com/Speedi13/RuntimeInstaller/blob/master/VsRuntimesInstaller/RedistLibrary.h#L102)
