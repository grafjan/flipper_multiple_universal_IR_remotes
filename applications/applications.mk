APP_DIR		= $(PROJECT_ROOT)/applications
LIB_DIR		= $(PROJECT_ROOT)/lib

CFLAGS		+= -I$(APP_DIR)
C_SOURCES	+= $(shell find $(APP_DIR) -name "*.c")
CPP_SOURCES	+= $(shell find $(APP_DIR) -name "*.cpp")


APP_RELEASE ?= 1
ifeq ($(APP_RELEASE), 1)
# Services
SRV_BT		= 1
SRV_CLI		= 1
SRV_DIALOGS	= 1
SRV_DOLPHIN	= 1
SRV_GUI		= 1
SRV_INPUT	= 1
SRV_LOADER	= 1
SRV_NOTIFICATION = 1
SRV_POWER	= 1
SRV_POWER_OBSERVER = 1
SRV_RPC = 1
SRV_STORAGE	= 1

# Apps
SRV_DESKTOP	= 1
APP_ARCHIVE	= 1
APP_GPIO = 1
APP_IBUTTON	= 1
APP_IRDA	= 1
APP_LF_RFID	= 1
APP_NFC		= 1
APP_SUBGHZ	= 1
APP_ABOUT	= 1
APP_PASSPORT = 1

# Plugins
APP_MUSIC_PLAYER = 1
APP_SNAKE_GAME = 1
APP_IMU_MOUSE = 1

# Debug
APP_ACCESSOR = 1
APP_BLINK = 1
APP_IRDA_MONITOR = 1
APP_KEYPAD_TEST = 1
APP_SD_TEST	= 1
APP_VIBRO_TEST = 1
APP_USB_TEST = 1
APP_DISPLAY_TEST = 1
APP_BLE_HID = 1
APP_USB_MOUSE = 1
APP_BAD_USB = 1
APP_U2F = 1
APP_UART_ECHO = 1
endif


# Applications
# that will be shown in menu
# Prefix with APP_*


APP_IRDA_MONITOR	?= 0
ifeq ($(APP_IRDA_MONITOR), 1)
CFLAGS		+= -DAPP_IRDA_MONITOR
SRV_GUI		= 1
endif


APP_UNIT_TESTS ?= 0
ifeq ($(APP_UNIT_TESTS), 1)
CFLAGS		+= -DAPP_UNIT_TESTS
endif


APP_ARCHIVE ?= 0
ifeq ($(APP_ARCHIVE), 1)
CFLAGS		+= -DAPP_ARCHIVE
SRV_GUI		= 1
endif


APP_BLINK ?= 0
ifeq ($(APP_BLINK), 1)
CFLAGS		+= -DAPP_BLINK
SRV_GUI		= 1
endif


APP_SUBGHZ ?= 0
ifeq ($(APP_SUBGHZ), 1)
CFLAGS		+= -DAPP_SUBGHZ
SRV_GUI		= 1
SRV_CLI		= 1
endif


APP_ABOUT ?= 0
ifeq ($(APP_ABOUT), 1)
CFLAGS		+= -DAPP_ABOUT
SRV_GUI		= 1
endif


APP_PASSPORT ?= 0
ifeq ($(APP_PASSPORT), 1)
CFLAGS		+= -DAPP_PASSPORT
SRV_GUI		= 1
endif


APP_LF_RFID ?= 0
ifeq ($(APP_LF_RFID), 1)
CFLAGS		+= -DAPP_LF_RFID
SRV_GUI		= 1
endif


APP_NFC ?= 0
ifeq ($(APP_NFC), 1)
CFLAGS		+= -DAPP_NFC
SRV_GUI		= 1
endif


APP_IRDA ?= 0
ifeq ($(APP_IRDA), 1)
CFLAGS		+= -DAPP_IRDA
SRV_GUI		= 1
endif


APP_VIBRO_TEST ?= 0
ifeq ($(APP_VIBRO_TEST), 1)
CFLAGS		+= -DAPP_VIBRO_TEST
SRV_GUI		= 1
endif


APP_USB_TEST ?= 0
ifeq ($(APP_USB_TEST), 1)
CFLAGS		+= -DAPP_USB_TEST
SRV_GUI = 1
endif

APP_UART_ECHO ?= 0
ifeq ($(APP_UART_ECHO), 1)
CFLAGS		+= -DAPP_UART_ECHO
SRV_GUI = 1
endif

APP_DISPLAY_TEST ?= 0
ifeq ($(APP_DISPLAY_TEST), 1)
CFLAGS		+= -DAPP_DISPLAY_TEST
SRV_GUI = 1
endif

APP_BATTERY_TEST ?= 0
ifeq ($(APP_BATTERY_TEST), 1)
CFLAGS		+= -DAPP_BATTERY_TEST
SRV_GUI = 1
endif

APP_USB_MOUSE ?= 0
ifeq ($(APP_USB_MOUSE), 1)
CFLAGS		+= -DAPP_USB_MOUSE
SRV_GUI = 1
endif

APP_BAD_USB ?= 0
ifeq ($(APP_BAD_USB), 1)
CFLAGS		+= -DAPP_BAD_USB
SRV_GUI = 1
endif

APP_U2F ?= 0
ifeq ($(APP_U2F), 1)
CFLAGS		+= -DAPP_U2F
SRV_GUI = 1
endif

APP_BLE_HID ?=0
ifeq ($(APP_BLE_HID), 1)
CFLAGS		+= -DAPP_BLE_HID
SRV_GUI = 1
endif

APP_KEYPAD_TEST ?= 0
ifeq ($(APP_KEYPAD_TEST), 1)
CFLAGS		+= -DAPP_KEYPAD_TEST
SRV_GUI		= 1
endif


APP_ACCESSOR ?= 0
ifeq ($(APP_ACCESSOR), 1)
CFLAGS		+= -DAPP_ACCESSOR
SRV_GUI		= 1
endif


APP_GPIO ?= 0
ifeq ($(APP_GPIO), 1)
CFLAGS		+= -DAPP_GPIO
SRV_GUI		= 1
endif


APP_MUSIC_PLAYER ?= 0
ifeq ($(APP_MUSIC_PLAYER), 1)
CFLAGS		+= -DAPP_MUSIC_PLAYER
SRV_GUI		= 1
endif

APP_SNAKE_GAME ?= 0
ifeq ($(APP_SNAKE_GAME), 1)
CFLAGS		+= -DAPP_SNAKE_GAME
SRV_GUI		= 1
endif

APP_IBUTTON ?= 0
ifeq ($(APP_IBUTTON), 1)
CFLAGS		+= -DAPP_IBUTTON
SRV_GUI		= 1
endif

APP_IMU_MOUSE ?= 0
ifeq ($(APP_IMU_MOUSE), 1)
CFLAGS		+= -DAPP_IMU_MOUSE
SRV_GUI		= 1
endif

# Services
# that will start with OS
# Prefix with SRV_*


SRV_BT ?= 0
ifeq ($(SRV_BT), 1)
CFLAGS		+= -DSRV_BT
SRV_CLI		= 1
endif


SRV_DESKTOP ?= 0
ifeq ($(SRV_DESKTOP), 1)
CFLAGS		+= -DSRV_DESKTOP
SRV_DOLPHIN	= 1
SRV_STORAGE	= 1
SRV_GUI		= 1
endif


SRV_DOLPHIN ?= 0
ifeq ($(SRV_DOLPHIN), 1)
CFLAGS		+= -DSRV_DOLPHIN
SRV_DOLPHIN_STATE_DEBUG ?= 0
ifeq ($(SRV_DOLPHIN_STATE_DEBUG), 1)
CFLAGS		+= -DSRV_DOLPHIN_STATE_DEBUG
endif
endif


SRV_POWER_OBSERVER ?= 0
ifeq ($(SRV_POWER_OBSERVER), 1)
CFLAGS		+= -DSRV_POWER_OBSERVER
SRV_POWER	= 1
endif


SRV_POWER ?= 0
ifeq ($(SRV_POWER), 1)
CFLAGS		+= -DSRV_POWER
SRV_GUI		= 1
SRV_CLI		= 1
endif

SRV_RPC ?= 0
ifeq ($(SRV_RPC), 1)
CFLAGS		+= -DSRV_RPC
ifeq ($(SRV_RPC_DEBUG), 1)
CFLAGS		+= -DSRV_RPC_DEBUG
endif
SRV_CLI		= 1
endif

SRV_LOADER ?= 0
ifeq ($(SRV_LOADER), 1)
CFLAGS		+= -DSRV_LOADER
SRV_GUI		= 1
# Loader autostart hook
LOADER_AUTOSTART ?= ""
ifneq ($(strip $(LOADER_AUTOSTART)), "")
CFLAGS		+= -DLOADER_AUTOSTART="\"$(LOADER_AUTOSTART)\""
endif
# Loader autostart hook END
endif


SRV_DIALOGS ?= 0
ifeq ($(SRV_DIALOGS), 1)
CFLAGS		+= -DSRV_DIALOGS
SRV_GUI		= 1
endif


SRV_GUI	?= 0
ifeq ($(SRV_GUI), 1)
CFLAGS		+= -DSRV_GUI
SRV_INPUT	= 1
SRV_NOTIFICATION = 1
endif


SRV_INPUT	?= 0
ifeq ($(SRV_INPUT), 1)
CFLAGS		+= -DSRV_INPUT
endif


SRV_CLI ?= 0
ifeq ($(SRV_CLI), 1)
CFLAGS		+= -DSRV_CLI
endif


SRV_NOTIFICATION ?= 0
ifeq ($(SRV_NOTIFICATION), 1)
CFLAGS		+= -DSRV_NOTIFICATION
endif


SRV_STORAGE ?= 0
ifeq ($(SRV_STORAGE), 1)
CFLAGS		+= -DSRV_STORAGE
endif
