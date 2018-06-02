/**
  @page Microsoft Azure IoT Cloud application

  @verbatim
  ******************************************************************************
  * @file    readme.txt 
  * @author  MCD Application Team
  * @brief   Description of the Azure Cloud application.
  ******************************************************************************
  *
  * Copyright (c) 2017 STMicroelectronics International N.V. All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  @endverbatim

@par Application Description 

The AzureXcubeSample application illustrates the various ways for an Azure device to interact
with an Azure IoT hub, using the Azure SDK for C low level interface and the Azure SDK for C serializer.

  Notes:
    - By contrast to the generic interface, the Azure low level interface lets the application explicitly
      schedule the network trafic.
      This allows the user to time-slice the network interface activity, and relieves from running
      a dedicated working thread for handling the network events. It can ease with a low-power SW design.
    - The serializer helps reliably building JSON strings to be sent to the IoT hub, from the device model
      attributes. It is not required - the sample applications provided by Microsoft in the Azure SDK for C
      do not use the serializer.


It supports the B-L475E-IOT01 board and connects to the Internet through the on-board network interface.


MbedTLS secures the network link on STM32 side.

The application connects to an Azure IoT hub thanks to the credentials provided
by the user on the console.



@par Hardware and Software environment

  - MCU board: B-L475E-IOT01 (MB1297 rev D), with FW "Inventek eS-WiFi ISM43362-M3G-L44-SPI C3.5.2.3.BETA9"
      Note: the FW version is displayed on the board console at boot time.

  - WiFi access point.
      * With a transparent Internet connectivity: No proxy, no firewall blocking the outgoing trafic.
      * Running a DHCP server delivering the IP and DNS configuration to the board.

  - Computer for running a device management application.
      * with a transparent Internet connectivity: no proxy, no firewall blocking the outgoing trafic.
      * This can be your development PC, a virtual private server, a single-board computer...
      * It may be connected to the same router as the MCU board.

  - Azure IoT account 
      * and create an IoT Hub.
        See https://docs.microsoft.com/en-us/azure/iot-hub/iot-hub-device-sdk-c-intro

  - An Azure device management application.
      * Required to communicate with the device.

      * Two options (can be downloaded from the link above):
        * Recommended: iothub-explorer
          - Runs on any operating system (node.js: Download the stable version from nodejs.org,
            as the package manager of your distribution may lag behind by several versions).
          - We selected this tool as the reference (in the readme and the user manual) for the present Cube expansion package.
        
        * Alternatively: Device Explorer
          - Runs on Windows (.NET)

  - A development PC for building the application, programming through ST-Link, and running the virtual console.


@par How to use it ? 

In order to make the program work, you must follow these steps:

Azure device creation
  - Create an Azure device and get its connection string.
      > iothub-explorer login <your Azure IoT Hub Connection String>
      > iothub-explorer create <devId> --connection-string
        For instance, you may want to use the MAC address of your board as a device ID, 
        or choose a smart nickname at your convenience.
        Keep a copy of the device connection string at hand: The AzureXcubeSample application
        provided by this package will request it on the console when it is launched for the first time.

  - Verify that you can retrieve the device twin status.
      > iothub-explorer get-twin <devId>


Application build and flash
  - Open and build the project with one of the supported development toolchains (see the release note
    for detailed information about the version requirements).

  - Program the firmware on the STM32 board: you can copy (or drag and drop) the generated ELF
    file to the USB mass storage location created when you plug the STM32 
    board to your PC. If the host is a Linux PC, the STM32 device can be found in 
    the /media folder with the name e.g. "DIS_XXXXX". XXXXX depends on the MCU model.
    For instance, if the created mass storage location is "/media/DIS_XXXXX", then the command to
    program the board with a binary file named "my_firmware.bin" is simply: cp my_firmware.bin 
    /media/DIS_XXXXX. 

   Alternatively, you can program the STM32 board directly through one of the 
   supported development toolchains.
 

Application first launch

  - Connect the board to your development PC through USB (ST-LINK USB port).
    Open the console through serial terminal emulator (e.g. TeraTerm), select the ST-LINK COM port of your
    board and configure it with:
    - 8N1, 115200 bauds, no HW flow control;
    - set the line endings to LF or CR-LF (Transmit) and LF (receive).

  - On the console:
    - Enter your Wifi SSID, encryption mode and password.

    - Set the device connection string (see above), without including the enclosing quotes (") or leading/trailing spaces.

    - Set the TLS root CA certificates: 
      Copy-paste the contents of Projects\Common\Azure\comodo_baltimore.pem.
      The device uses them to authentify the remote hosts through TLS.

      Note: The AzureXcubeSample application requires that a concatenation of 2 CA certificates is provided
          1) for the HTTPS server which is used to retrieve the current time and date at boot time.
             (the "Comodo" certificate).
          2) for the IoT hub server (the "Baltimore" certificate).
          The concatenated string must end with an empty line. This is comodo_baltimore.pem.

      Note: The Baltimore CA authentifies the azure-devices.net domain. 
            If your IoT hub belongs to a different domain, you may have to select and configure a
            different root CA certificate.
  
     - After the parameters are configured, it is possible to change them by restarting the board 
     and pushing the User button (blue button) just after boot.


Application runtime 
  - Makes an HTTPS request to retrieve the current time and date, and configures the RTC.

      Note: HTTPS has the advantage over NTP that the server can be authentified by the board, preventing
            a possible man-in-the-middle attack.
            However, the first time the board is switched on (and each time it is powered down and up, if the RTC 
            is not backed up), the verification of the server certificate will fail as its validity period will
            not match the RTC value.
            The following log will be printed to the console: "x509_verify_cert() returned -9984 (-0x2700)"

            If the CLOUD_TIMEDATE_TLS_VERIFICATION_IGNORE switch is defined in cloud.c (it is the case by default),
              this error is ignored and the RTC is updated from the "Date:" field of the HTTP response header.
            Else, a few more error log lines are printed, and the application retries to connect without verifying
              the server certificate.
              If/once ok, the RTC is updated.
            
  - Connects to the Azure IoT hub,
      - Gets the status of the device twin;
      - Updates its local properties (DesiredTelemetryInterval) from the device twin "desired properties";
      - Reports the "reported properties" to the device twin (TelemetryInterval and LedStatus).

    Note: From this point, the user can get the twin status updates through:
      > iothub-explorer get-twin <devId>

  - Stays idle, pending on local user, or hub-initiated events.

    - Possible local user actions:

      - Single push on the user button:
            Trigs a telemetry message publication to the IoT hub through a DeviceToCloud (D2C) message.

      - Double push on the user button:
            Starts or stops the telemetry publication loop.
            When the loop is running, the telemetry messages are published every TelemetryInterval seconds.
      
      Note: New sensor values are sampled for each telemetry message, and a timestamp is attached.
      Note: Each telemetry message publication is signaled by the user LED blinking quickly for half a second.
    
    - Possible hub-initiated events:

      - CloudToDevice (C2D) message:
          The message is displayed on the board console.

      - C2D twin update:
          Change of the telemetry publication period (DesiredTelemetryInterval). 

      - C2D method:
          Call one of the following device methods: 
            * Reboot: Reboot the board.
            * Hello: Display the message passed as parameter on the board console.

      - C2D action call:  
            * LedToggle: Toggle the user LED state.


    The companion document Projects\Common\Azure\ReadmeRefs.htm details:
      - The runtime state flow (Fig. 1);
      - The iothub-explorer command lines for the user to trig hub-initiated events, and see the results (Table 1).


@par Directory contents

---> in .
Inc
  es_wifi_conf.h                     Es_Wifi configuration.
  es_wifi_io.h                       Functions prototypes for es_wifi IO operations.
  flash.h                            Management of the internal flash memory.
  main.h                             Header containing config parameters for the application and globals.
  stm32l4xx_hal_conf.h               HAL configuration file.
  stm32l4xx_it.h                     STM32 interrupt handlers header file.
  vl53l0x_platform.h                 vl53l0x proximity sensor platform include file.
  vl53l0x_proximity.h                vl53l0x proximity sensor include file.
  wifi.h                             Wifi core resources definitions.

Src
  es_wifi_io.c                       Es_Wifi IO porting.
  flash_l4.c                         Flash programing interface.
  main.c                             Main application file.
  stm32l4xx_hal_msp.c                Specific initializations.
  stm32l4xx_it.c                     STM32 interrupt handlers.
  system_stm32l4xx.c                 System initialization.
  vl53l0x_platform.c                 vl53l0x proximity sensor platform file.
  vl53l0x_proximity.c                vl53l0x proximity sensor.
  wifi.c                             WiFi-LL interface.

---> in Projects/Common/Azure
comodo_baltimore.pem                 List of root CA certificates to be pasted on the board console at first launch.
                                     Note: Baltimore is the root CA for the azure-devices.net domain.
                                     If your IoT hub belongs to a different domain name, you may have to use a different CA. 

ReadmeRefs.htm                       Readme figures.
ReadmeRefs_files/*                   Readme figures.

Inc
  az_mbedtls_config.h                   Application-specific mbedTLS middleware configuration.
  az_tlsio_mbedtls.h                    Azure SDK tlsIO adapter interface declaration.
                                          Overloads the SDK adapter file in order not to include azure_c_shared_utility/tls_config.h.

Src
  az_platform_STM32Cube.c               Routing of the Azure SDK required interface.
                                          NB: platform_init() and platform_deinit() are provided by cloud.c.
  az_socketio_mbed.c                    Implementation of the Azure SDK socket adapter interface.
                                          NB: Reuse the mbed version from the SDK, with minimal changes so that it is
                                          compatible with the mbedtls tlsio version.
  az_tcpsocketconnection_c_STM32Cube.c  Network port for the mbed socket adapter.
  az_threadapi_STM32Cube.c              Implementation of the Azure SDK thread adapater interface. No multithread support is currently implemented.    
  az_tickcounter_STM32Cube.c            Implementation of the Azure SDK tick counter adapter interface.
  az_tlsio_mbedtls.c                    Implementation of the Azure SDK tlsIO adapter interface.
                                          NB: Overloads the SDK adapter file in order not to include "azure_c_shared_utility/tlsio_mbedtls.h".
  azure_base64.c                        Avatar of the Azure SDK base64.c file (filename conflict with mbedTLS).
  azure_sha1.c                          Avatar of the Azure SDK sha1.c file (filename conflict with mbedTLS).
  azure_version.c                       Avatar of the Azure SDK version.c file (filename conflict with mbedTLS).
  AzureXcubeSample.c                    Application implementation.

---> in Projects/Common/Shared
Inc
  cloud.h
  heap.h
  http_util.h
  iot_flash_config.h
  mbedtls_net.h
  msg.h
  net.h
  net_internal.h
  rfu.h
  sensors_data.h
  timedate.h
  timingSystem.h
  version.h

Src
  cloud.c                             Platform init and deinit functions.
  entropy_hardware_poll.c             RNG entropy source for mbedTLS.
  heap.c                              Utility functions to measure the heap size.
  http_util.c                         Helpers for building HTTP requests, and downloading by chunks.
  iot_flash_config.c                  Dialog and storage management utils for the user-configured settings.
  mbedtls_net.c                       Network adpater for mbedTLS on NET.
  net.c                               Network socket API.
  net_tcp_wifi.c                      NET TCP / WiFi-LL implementation.
  net_tls_mbedtls.c                   NET TLS / mbedTLS implementation.
  rfu.c                               Firmware versioning and change management.
  sensors_data.c                      Board-specific file to retrieve and format sensors data.
  STM32CubeRTCInterface.c             Libc time porting to the RTC.
  timedate.c                          Initialization of the RTC from the network.
  timingSystem.c                      Libc time porting to the RTC.
  wifi_net.c                          WiFi_LL init/deinit functions.

@Par Target-specific notes   

@par Caveats

  - The mbedTLS configuration parameter MBEDTLS_SSL_MAX_CONTENT_LEN is tailored down to 5kbytes.
    It is sufficient for connecting to the Azure IoT cloud, and to the HTTPS server used for retrieveing
    the time and date at boot time.
    But the TLS standard may require up to 16kbytes, depending on the server configuration.
    For instance, if the server certificate is 7kbytes large, it will not fit in the device 5kbytes buffer,
    the TLS handshake will fail, and the TLS connection will not be possible.

  - Beware the leading and trailing characters when entering the device connection string on the console. The string ends with '='.


@par Troubleshooting

  - Hardfault during the mbedTLS initialization
    * Check the contents of the root CA configured by the user.

	- mbedTLS handshake failure
		* on mbedtls_pk_sign()
			 - Undetected heap overflow.
		

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
