ASM_OBJ += 

# Makefile.cms ‚Å’è‹`‚µ‚Ä‚¢‚é‚Ì‚Å‚±‚±‚Å‚Í•s—v
#C_OBJ += $(XCUBE_LIB_DIR)/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/system_stm32l4xx.o 

C_OBJ += \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/es_wifi_io.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/wifi.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/flash_l4.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/stm32l4xx_hal_msp.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/vl53l0x_platform.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/vl53l0x_proximity.o \

#		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/main.o \
		${XCUBE_LIB_DIR}/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Src/stm32l4xx_it.o \

CXX_OBJ += 

INCLUDE_PATHS += -I. \
		-I$(XCUBE_LIB_DIR)/Projects/B-L475E-IOT01/Applications/Cloud/Azure/Inc


