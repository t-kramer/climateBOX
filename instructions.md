# climateBOX Building Instructions

# List of components
### a. Electronics
  1x ESP32 DevKit<br>
  1x PCB<br>
  1x DHT-22<br>
  1x DS18b20<br>
  1x revC<br>
  1x SD module<br>
  1x RTC*<br>
  1x OLED display*<br>
  4x 10k Ohm resistors<br>
  2x 15k Ohm resistors<br>
  1x SD card/microSD card with adapter
  1x standard micro-USB cable<br>
  1x standard 5V wall charging adapter**<br>
  
### b. Consumables
  - Solder
  - Stranded Wire (e.g. 22 AWG); potentially in multiple colors
  - Feather stacking headers, female
  - Breakaway headers, male
  - Crimp connectors + housing
  - Heat shrink tubes
  - Solder wick / Solder vaccuum
  
### c. Housing
  1x Main element (side walls)<br>
  1x Bottom element<br>
  1x Top element<br>
  1x PCB frame<br>
  1x revC socket<br>
  1x revC cover<br>
  1x Ping-pong ball, matt-black<br>
  1x Display dummy*<br>
  
### d. Equipment
  - Soldering Iron<br>
  - Screw driver<br>
  - Drill<br>
  - Crimping Tool<br>
  - Super glue<br>
  
  - +only relevant for 'display-version'<br>
  - ++optional<br>
  ----------------------------------
  # Step-by-Step Instructions
  ## 1. Preparing the PCB
  #### 1.1 Attaching the resistors to PCB
  #### 1.2 Attaching female feather headers to PCB
  ESP32, DHT
  #### 1.3 Attaching male headers to PCB
  revC, DS18b20
  
  ## 2. Preparing components
  #### 2.1 DS18b20
  - Shorten wire
  - Attach crimp pins
  - Attach crimp housing
  - Use heat shrink to protect soldered connections
  
  #### 2.2 revC
  - cut wires (length = )
  - tin one end of wires
  - at other end attach crimp connection (pins, housing)
  - put solder on revC pin-holes
  - slip over heat shrink
  - reheat solder and lightly press down tinned wire ends
  - Use heat shrink to protect soldered connections
  
  #### 2.3 RTC
  - potentially remove any pre-installed pins
  - cut wires (length = )
  - tin one end of wires
  - at other end attach crimp connection (pins, housing)
  - slip over heat shrink
  - lead tinned wire ends through RTC module pin holes and solder
  - Use heat shrink to protect soldered connections

  #### 2.4 SD Module
  - potentially remove any pre-installed pins
  - cut wires (length = )
  - tin both ends of wires
  - lead tinned wire ends through SD module pin holes and solder
  - slip over heat shrink
  
  ## 3. Preparing the housing
  - test-assemble parts
  - drill holes through top and bottom elements at pre-cut positions
  - pre-drill holes in housing main element
  - if in step XX the PCB frame and bottom element did not click together properly, pontentially use glue to fix connection
  - use screwdriver to tighten screws through pre-drilled holes
  
  
--------------------------------------------------  
  ## 4. Assembly
  #### 4.1 Core assembly
  #### 4.1.1 DS18b20
  - attached prepared DS18b20 sensor to PCB via crimp connectors
  
  #### 4.1.2 revC
  - attached prepared revC sensor to PCB via crimp connectors
  
  #### 4.1.3 RTC
  - lead tinned wire ends through pin hole and solder
  - Use heat shrink to protect soldered connection
  
  #### 4.1.4 SD Module
  - lead other end of SD module wires through PCB pin holes and solder
  - Use heat shrink to protect soldered connection
  - makes sure SD card is inserted

  #### 4.1.5 DHT22
  - attach DHT22 to PCB using pre-installed feather headers
  
  
  #### 4.2 Connection test
  #### 4.2.1 Prepare Arduino IDE
  Quick Ardunio IDE overview here: LINK<br>
  - Install ESP32 via board manager
  - Install pre-required libraries 

  #### 4.2.2 Test Connection
  - Explanation test code
  - Upload the test.ino file

  #### 4.3 Final assembly
  - detach DS18b20 & revC
  - lead wires of both sensors through pre-cut holes in top element
  - click PCB with SD and RTC module into PCB frame element
  - click PCB frame element into pre-cut holes in bottom element (maybe use glue to fix connections)
  - click in DHT22 sensor into PCB designated feather headers
  - slip over main wall element
  - lift top element including revC and DS18b20 sensors
  - connect sensors to PCB
  - while pushing wires into the interior of the box, push down the top element and attach by tightening screws
  - attach USB wires


--------------------------------------------------
  ## 5. The Program Code
  - Explanation climateBOX code
  - Highlight differences
  - Make sure that: USB wire is attached properly, SD card is inserted
  - connect climateBOX to PC/Mac via USB
  - Upload the climateBOX_rtc.ino file
