##Overview
MGUI is a C++ library that wraps over low level Windows GUI API. The library is minimal
and not cross platform. It's purpose is to provide a way to quickly
create Windows GUI apps using the MingW system and without using Visual C++.

Because mgui simply wraps over native UI, apps will look good and perform well.

If you need cross platform support, there are better options like
Qt.

###Build Instructions

Need to link your application code with the following libraries:

```
-lmgui -lcomctl32 -lgdi32 -lcomdlg32
```

###Resource File Guide
View the [resource file guide] (resource.md).
