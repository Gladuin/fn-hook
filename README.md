# fn-hook
Utility to customise the features that the "Fn and Function keys" driver for certain Lenovo laptops provides.

# Features
 - Customize what program the "s star" button launches (without useless menu like the one Lenovo provides)
 - Customize what OSD popups show up
 
# Usage
 1. Install [this version of the driver](https://www.catalog.update.microsoft.com/ScopedViewInline.aspx?updateid=7769e97c-3abd-49a2-8e70-8e185f47523f) [(link to .cab)](https://catalog.s.download.windowsupdate.com/d/msdownload/update/driver/drvs/2022/02/9e850dca-f8e1-4701-aa71-fe94f1955726_410cca1786d5e0222e92c13ea7e3584310742be5.cab)
 2. Install [Visual C++ Redistributable 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145)
 3. Extract the latest .zip from the [releases page](https://example.com) somewhere
 3.1. (OPTIONAL) Your antivirus will probably detect `injector.exe` as a virus, because of the way it works. If this is the case, add the extracted folder to your antiviruses' exceptions.
 4. Run `injector.exe`

If you want the injector to start on boot, run `add_to_startup.bat`.

The config doesn't get reloaded until you re-inject `hook.dll`, so while `injector.exe` is still running run `reload.bat` to reload your changed config.

# Notes
I've only tested this with a Lenovo IdeaPad 5 Pro 14ACN6, but it should work fine on any laptop that the "Fn and Function keys" v2.0.0.0 driver works properly on.