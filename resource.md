## Guide to Developing Windows Resource Files

1. A resource file should be compiled as:
```
	windres -i resource.rc -o resource.o
```

2. The resource file must include windows.h.

3. Creating a basic dialog:

```
MyDialog DIALOG 6, 18, 180, 180
STYLE DS_MODALFRAME | WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU
CAPTION "Dialog Title"
BEGIN
	//List of controls
END
```

4. How to add a text box?

```
    EDITTEXT 10001, 44, 65, 32, 12, ES_AUTOHSCROLL
```

Multiline:

```
    EDITTEXT 10001, 44, 65, 32, 12, ES_MULTILINE | ES_WANTRETURN | ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL
```

5. How to add a label?

```
    LTEXT           "Text", 10002, 33, 18, 70, 15
```

6. How to add check box?
```
    AUTOCHECKBOX    "Check", 10003, 17, 116, 40, 10
```

7. Add a button

```
    PUSHBUTTON      "Push", IDOK, 77, 142, 40, 14
    DEFPUSHBUTTON   "Push", IDOK, 77, 142, 40, 14
```

8. Editable combo:
```
    COMBOBOX        10004, 52, 89, 100, 81, CBS_DROPDOWN | WS_VSCROLL | WS_TABSTOP
```
The height is the height of the drop down.

9. Non-editable combo:
```
    COMBOBOX        10004, 52, 89, 100, 81, CBS_DROPDOWNLIST | WS_VSCROLL | WS_TABSTOP
```

10. Add a list box:

```
	LISTBOX 10005, 5, 30, 40, 40 , ES_AUTOVSCROLL | ES_AUTOHSCROLL | WS_VSCROLL | LBS_NOTIFY | WS_BORDER
```

11. Add a menu bar to the resource file:

```
MainMenu MENU DISCARDABLE
BEGIN
     POPUP "&File"
     BEGIN
         MENUITEM "&Open ... Ctrl+O", _OPEN
         MENUITEM "&Save", _SAVE
         MENUITEM SEPARATOR
         MENUITEM "E&xit", 10001
     END
     POPUP "&Tools"
     BEGIN
         MENUITEM "&New control", 10002
         MENUITEM "&Select", 10003
         MENUITEM "&Properties", 10004
     END
END
```

12. Add a date time picker:

```
	CONTROL "", ID_PAID_DATE, "SysDateTimePick32", 0, 4, 60, 88, 12
```
