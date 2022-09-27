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
    e.g. 
  - Stranded Wire (e.g. 22 AWG); potentially in multiple colors
    e.g. 
  - Feather stacking headers, female
    e.g. 
  - Breakaway headers, male
    e.g. 
  - Crimp connectors + housing
    e.g. 
  - Heat shrink tubes
    e.g. 
  - Solder wick / Solder vaccuum
    e.g. 
  
### c. Housing
  1x Main element (side walls)<br>
  1x Bottom element<br>
  1x Top element<br>
  1x PCB frame<br>
  1x revC socket<br>
  1x revC cover<br>
  1x Ping-pong ball, matt-black<br>
  1x Display dummy+<br>
  
### d. Equipment
  - Soldering Iron<br>
  - Screw driver<br>
  - Drill<br>
  - Crimping Tool+<br>
  - Super glue<br>
  
  - +only relevant for 'display-version'<br>
  - ++optional<br>
  ----------------------------------
  # Step-by-Step Instructions
  ## 1. Preparing the PCB
  #### 1.1 Attaching the resistors to PCB
  The first step is to attached the six resistors (4x10Ohm,2x15Ohm) to the PCB. For each resistor, lead both ends through the dedicated Through Holes (TH) on the top side of the board. Then, turn the board around and fix the resistors by soldering it to the board.
  #### 1.2 Attaching female feather headers to PCB
  As a next step you should attach the female feather headers for ESP32 and DHT-22 to the PCB. Alternatively, you could also directly solder ESP32 and DHT-22 to the PCB, using headers however simplifies the disassembly in case of component errors or other problems.
  #### 1.3 Attaching male headers to PCB
  Similar to the previous step, for revC and DS18b20 for opted for the flexible solution of not directly soldering these sensors or their wire ends to the PCB, but to use headers instead. In this case we used the combination of male headers on the PCB side and crimped female connecters on the sensor side. More about that in Section 2.
  
  ## 2. Preparing components
  #### 2.1 DS18b20
  The DS18b20 usually comes with a relatively long cable containing three wires for power (red), ground (black) and data (white). We recommend shortening the cables/wires to around 150mm. But be careful here not to remove too much cable. The sensor/globe thermometer of the climateBOX sits on top of a small pole. The length of the pole needs to be included in the cable length, otherwise the cable won't be long enough and you can't connect it to the PCB.<br>
  <br>
  Now, after you have shortened the wires, it is time to attached the female connectors. But before you can proceed you need to get rid of some insulation to expose the wires, a process called stripping. If you have never stripped a wire before, this might be of help:
  https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire<br>
  <br>
  After removing the insulation for both the main cable and the individual wires, the crimping pins can be attached to the three wires using a crimping tool. Here, you should have a few trial runs before you actually start using the crimp connectors on the DS18b20 wires. Again, here is a helpful link that can get you started:
  https://core-electronics.com.au/guides/prototyping/how-to-crimp-custom-connectors/<br>
  <br>
  If you are satisfied with your crimping connections, go ahead and push the female connectors into the housing. Here, make sure that you keep the wires in the right order (attached photo) and that the crimped connectors are pushed deep enough into the housing until they properly click in. But be careful, as soon the connectors are locked it can be hard to get them out again without breaking the plastic housing. If you like it neat and tidy, you can also use some heat shrink tube to protect the exposed wires. Make sure that you push the raw, non-shrinked tube over the cable before you conclude this step by attaching the housing. Then you can use a hairdryer 
  
  #### 2.2 revC
  Now it's time to prepare the the revC. Here, we are using only four of the five through holes: GND, TMP, RV, VCC (?). The first step here is cut four wires (AWG22) to a length of approximately ... mm. Colour coding makes sense here, so if you have wire of different colours go ahead and use them. In the example, we used black/GND, red/VCC, yellow/XXX and green/XXX. Then, you can strip both ends of the wires. Again, on one end of each wire, a female crimp connector is attached. Before these connectors are pushed into the housing, you need to tin the other end of the wires. Tinning is described under the same link we used earlier:<br>
  https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire<br>
  <br>
  After you've completed these steps, make sure that all wires have approximately the same length. This is important for the next step. Also, you can slip over some heat shrink over each wire. Like previously, we will use that later to protect the soldered connections. 
	
Now we solder the prepared wires to the actual revC module. In the final climateBOX unit, the revC is positioned upright in a special socket. This is why you shouldn't follow the usual process of using the through holes for the soldering connection. If we do that, the wires will be attached in a 90° angle. The socket doesn't provide enough space for that and there would be too much strain on the wire-sensor connection. Here, we will use an alternative method that allows use to solder the wires "in-parallel", which is more space-effective in this scenario. To do that, we start by applying a big drop of solder to the connections we need. The solder needs to fully cover each through hole that we need, you can be generous here. Then, wire for wire, you reheat the "solder-drop" and gently push the tinned wire-ends into the melted solder and let it cool down to fix the connection. In the end, you only need to shrink the protecting tubes. Voila, we've got ourselves a good and reliable connection. 
  
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
  All housing parts can be 3D printed by using the .stl files here in the repository. When printed, you should go ahead and check every part for potential printing errors and afterwards test-assemble everything. The explosion graphic below shows you how to do that.
  
  **Explosion Graphic**
<img src='https://user-images.githubusercontent.com/49641232/192633547-98402b64-37cd-466a-a59b-177db0aa44fd.png' width="600" height="900">

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
