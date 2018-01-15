define Profile/GLORIA
	NAME:=GLORIA V1 WIFI BOARD
	PACKAGES:=kmod-usb-core kmod-usb2 kmod-ledtrig-usbdev rssileds
endef

define Profile/GLORIA/Description
	Package set optimized for the GLORIA V1.
endef

$(eval $(call Profile,GLORIA))
