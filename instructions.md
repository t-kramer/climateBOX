# climateBOX Building Instructions

# List of components
### a. Electronics
  1x ESP32 DevKit<br>
  1x PCB<br>
  1x DHT-22<br>
  1x DS18B20<br>
  1x revC (Modern Device)<br>
  1x SD module<br>
  1x RTC module*<br>
  1x OLED display*<br>
  4x 10k Ohm resistors<br>
  2x 15k Ohm resistors<br>
  1x SD card/microSD card with adapter
  1x standard micro-USB cable<br>
  1x standard 5V wall charging adapter**<br>

<img src='https://user-images.githubusercontent.com/49641232/197687118-3cef56c6-803c-42b8-9e1c-8e7bd952123f.png' width="75%" height="75%">

<br>
  
### b. Consumables
  - solder
  - stranded Wire (e.g. 22 AWG); potentially in multiple colors
  - solid core wire*
  - feather stacking headers, female
  - breakaway headers, male
  - crimp connectors + housing
  - heat shrink tubes*
  - solder wick / solder vaccuum
    
<img src='https://user-images.githubusercontent.com/49641232/197735830-797ab102-67d4-4b23-971d-f8e800ba0475.png' width="50%" height="50%">
  
### c. Housing
  1x main element (side walls)<br>
  1x bottom element<br>
  1x top element<br>
  1x PCB frame<br>
  1x revC socket<br>
  1x revC cover<br>
  1x ping-pong ball, matt-black<br>
  1x display dummy* (only for v0.0)<br>
  
 <img src='https://user-images.githubusercontent.com/49641232/215630392-d68c3e94-9fc1-4404-a4cd-8200f0625396.png' width="50%" height="50%">
  <br>
  
  
### d. Equipment
  - soldering Iron<br>
  - screw driver<br>
  - drill*<br>
  - crimping tool* (non-essential, but recommended)<br>
  - super glue*<br>

<br>
<br>
  
### e. Pre-requisites
<br>
**a) Order components**

You will not be able to build the climateBOX without the necessary components. Please stick to component list above and order them online. I can personally recommend two stores:

- https://core-electronics.com.au - Core Electronics is a good choice; generally a bit pricier, but quality equipment and fast delivery <br>
- https://au.banggood.com - Banggood is another store I can recommend; they ship internationally and are usually much cheaper; delivery takes some time though <br>

If you have questions about the components and have problems getting hands on them, feel free to send us a message and or use the discussions page. We are happy to help!
<br>
<br>

**b) Order the PCB(s)**

Before you can start building and using the climateBOX, you need to order the PCBs, or printed circuit boards. You find all require files in the following folder: 

*/pcb* REPLACE by link

I recommend pages like https://jlcpcb.com. There are many alternatives and usually they follow the same process. You just upload the PCB files, also called “Gerber” files. This will provide the manufacturer with all essential information. You can then specify the quantity of PCBs you want to order and can make some custom configurations like changing the colour of the board etc. The price per board should be less than 2 USD.<br>
Please check the following configurations before you order:
-	Dimensions: 80mm x 60mm
-	Layers: 2

Otherwise, you should be ready to go!

*Also, if you are in Australia, feel free to contact us. We have some PCBs left and can post them.*
<br>
<br>

**c) Install Arduino IDE**

The Arduino IDE is the software we are going to use to upload the climateBOX code to the microcontroller. If you want, you can install the software before you get started with the assembly. More information can be found further below in **PART III** of the instructions.
<br>
Also, for newbies, here is a collection of useful links and tutorials that will help you with some essential vocabulary, explanations, and techniques. We will update this selection along the way.

soldering:<br>
https://core-electronics.com.au/guides/soldering-101/

stripping:<br>
https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire

tinning:<br>
https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire

crimping:<br>
https://core-electronics.com.au/guides/prototyping/how-to-crimp-custom-connectors/

stranded vs solid core wire:<br>
https://learn.sparkfun.com/tutorials/working-with-wire/stranded-vs-solid-core-wire

'art' of desoldering:<br>
https://www.instructables.com/How-to-Desolder-Headers/

Arduino IDE:<br>
https://core-electronics.com.au/guides/arduino-ide-tutorial/

installing ESP32 board to Arduino IDE:<br>
https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
https://www.arduino.cc/en/software

Installing Arduino libraries:<br>
https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries

Upload program to microcontroller:<br>
https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE

<br>
<br>
<br>

  
  ----------------------------------
  # Step-by-Step Instructions
Now, that you hopefully have taken care of all the pre-requisites above, let’s get to the core: the actual assembly.<br>
The instructions are separated into three sections. For the first two sections, there is a helpful **video** that visualises the steps.<br> Please be aware that videos are not updated regularly and should be seen as supplementary material. **Always refer to written instructions here as the main source for documentation.**<br>
The links to the videos can be found here:<br>
*DIR* UPDATE LINK
<br>
<br>
Before you start, again make sure you have done the following:
- you’ve got the PCB
- you have 3D-printed the housing parts
- you have all the components, consumables and equipment required (here we have given an asterisk * to non-essential items in the lists above)
<br>
Below you can see the circuit diagram, your main resource for building the climateBOX. Please be aware, that for v0.0 you can simply swap the OLED display for the RTC module. Both components uses the same interfaces.
<br>
<img width="1028" alt="circuit-diagram" src="https://user-images.githubusercontent.com/49641232/215632832-81c7f898-cc51-4e25-b9b7-b22c9011c00e.png">
<br>
The ESP32 as the microcontroller is the heart of the device. Three sensors (DHT-22, DS18B20, revC) and two additional components (SD module, OLED display/RTC module) are connected to the ESP32. Both the DHT-22 and DS18B20 require a pull-up resistor (R5, R6). As you can see, the diagram for the revC looks a bit more complicated. Here we need to do some level shifting. The revC requires around 5V to operate and provides an output signal in the same range. The ESP32 however runs on 3.3V. So to be able to use the revC output, we need to bring it down to the ESP32 operating range.
<br>
Download here:<br>
[schematic-v1.1.pdf](https://github.com/t-kramer/climateBOX/files/10541439/schematic-v1.1.pdf)

  
  
## 1. PART I: Preparing the PCB
  
#### PCB Overview
  
<br> 
<img src='https://user-images.githubusercontent.com/49641232/215633430-0421d621-c958-422b-b8b6-9dd0039d7652.png' width="50%" height="50%">
<br>
The printed circuit board (PCB) is the heart of the climateBOX. All components will be connected to the PCB, either directly soldered or via headers. We decided to use a hybrid approach in which we used headers for the sensors and the direct solder connection for other components. The obvious advantage of headers is that it is a lot easier to attach and detach components during assembly and in the case that a component needs to be replaced.<br>
<br>
The following instructions will roughly follow the order shown in the video. It is best to follow both the written instructions and the video, especially when you are new to microelectronics.<br>
<br>
**NOTE: This part is covered in video ‘climateBOX - PART I’**
<br>
The link to the video can be found here:
<br>
LINK
<br>
  
#### 1.1 Attaching the resistors to PCB
The first step is to attach the six resistors (4x10k Ohm,2x15k Ohm) to the PCB. For each resistor, thread both ends through the dedicated ‘through holes’ from the top side of the PCB. Then, turn the board around and fix the resistors by soldering both ends to the board.<br>
<br>
  
<br> -- IMAGE -- <br>
  
#### 1.2 Attaching female feather headers to PCB
Next, you can attach the female feather headers for the ESP32 to the PCB. Alternatively, you could also directly solder ESP32 to the PCB. However, like explained before, using headers will simplify the disassembly in case of component errors or other problems.<br>
<br>
  
<br> -- IMAGE -- <br>
  
#### 1.3 Attaching male headers to PCB
Like in the previous step, for revC and DS18b20 we opted for the flexible solution of not directly soldering these sensors or their wire ends to the PCB, but to use headers instead. In this case we used the combination of male headers on the PCB side and crimped female connecters on the sensor side. More about that in Section 2. So go ahead and solder the male pin headers to the PCB.<br>
<br>
  
<br> -- IMAGE -- <br>
  
#### 1.4 DHT-22
If you want, you can also solder the DHT-22 to the PCB directly by using the dedicated holes in the middle of the PCB. You can also use female feather headers. This will give you more flexibility when attaching/detaching the sensor.<br>
<br>

#### 1.5 RTC | Display module
Attention: If you opted for v1.0 that uses the display, I do recommend adding another set of male pin headers to the PCB. This will facilitate the assembly of climateBOX v1.0 later. This step is not covered in the video but follows the same procedure.<br>
<br>
That’s all you have to do to prepare the PCB. Next, let’s take care of the individual components and prepare them for assembly.<br>
<br>
<br>
  
## 2. PART II: Preparing components
The following components are covered in this section:
-	globe temperature sensor (DS18B20)
-	air velocity sensor (revC)
-	SD module
<br>
Depending on the version you want to build, you have to add one additional component to this list. If you are going for v0.0, you have to prepare the RTC module. Otherwise, if you are going for v1.0, prepare the display module. Since the preparation of both modules pretty much follows the same steps covered earlier in this section, we do not cover them in the video. But you should have enough practice from previous steps to prepare these components without visual instructions.<br>
<br>
**NOTE: This part is covered in video ‘climateBOX - PART II’**
<br>
The link to the video can be found here:
<br>
LINK
<br>

#### 2.1 DS18B20
The DS18b20 usually comes with a relatively long cable containing three wires for power (red), ground (black) and data (white). We recommend shortening the cables/wires to around 150mm. But be careful here not to remove too much. The sensor/globe thermometer of the climateBOX sits on top of a small pole. The length of the pole needs to be included in the cable length, otherwise the cable won't be long enough, and you cannot connect it to the PCB.
<br>
  
   <br> -- IMAGE -- <br>
  
<br>
Now, after you have shortened the wires, it is time to attach the female crimp connectors. But before you can proceed you need to get rid of some insulation to expose the wires, a process called stripping. If you have never stripped a wire before, this might be of help:
<br>
https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire
<br>
  
   <br> -- IMAGE -- <br>
  
<br>
After removing the insulation for both the main cable and the individual wires, the crimping connectors can be attached to the three wires using a crimping tool. Here, you should have a few trial runs before you actually start using the crimp connectors on the DS18B20. Again, here is a helpful link that can get you started:<br>
<br>
https://core-electronics.com.au/guides/prototyping/how-to-crimp-custom-connectors/
<br>
  
   <br> -- IMAGE -- <br>
  
  <br>
If you are satisfied with your crimping skills, go ahead apply them on the three wires.<br>
<br>
If you like it neat and tidy like me, now is the time to use some heat shrink tube to protect the exposed wires.<br>
<br>
When you are done, push the female connectors into the housing. Here, make sure that you keep the wires in the right order and that the crimped connectors are pushed deep enough into the housing until they click in properly. But be careful, as soon the connectors are locked in it can be quite hard to get them out again without breaking the plastic housing.<br>
<br>
Then you can use a hairdryer to apply the heat shrink. Done!<br>
<br>

<br> -- IMAGE -- <br>

<br>
#### 2.2 revC
Now it's time to prepare the revC. Here, we are using only four of the five pins: GND, TMP, RV, +V.<br>
<br>
The first step here is cut four wires (I recommend stranded wire, AWG22) to a length of approximately 100 mm.<br>
<br>

<br> -- IMAGE -- <br>

Stranded wire is best here because it will be exposed to some bending. Stranded wire is more flexible and is generally the best choice for most internal components.<br>
<br>
Read more about the difference between strand and solid core wire here:
<br>
https://learn.sparkfun.com/tutorials/working-with-wire/stranded-vs-solid-core-wire
<br>
Also, colour coding makes sense here. If you have wire of different colours go ahead and use them. In the example, we used black/GND, red/V+, yellow/RV and green/TMP.<br>
<br>

<br> -- IMAGE -- <br>

Then, you can strip both ends of the wires.<br>
<br>
On one end of each wire, a female crimp connector will be attached. Here, you can go through the same process we described earlier: stripping, crimping female connectors, pushing connectors into the housing.<br>
<br>
On the other end, we need to the tin the stranded wire. The process of tinning is described under the same link we used earlier:
<br>
https://learn.sparkfun.com/tutorials/working-with-wire/how-to-strip-a-wire<br>
<br>
After you've completed these steps, make sure that all wires have approximately the same length. This is important for the next step. You can use a side cutter to adjust the length of the tinned wire ends.
Also, you can go ahead slip over some heat shrink over each wire. Like previously, that will later protect the soldered connections.<br>
<br>	
Now you can solder the prepared wires to the actual revC module. In the final climateBOX unit, the revC is positioned upright in a special socket (part 6+7 in the explosion graphic). Therefore, you shouldn't follow the usual process of using the through holes for the soldering connection. If you do that, the wires will be attached in a 90° angle. The socket doesn't provide enough space for that and there would be too much strain on the wire-sensor connection. Here, we will use an alternative method that allows use to solder the wires "in-parallel", which is more space-efficient in this scenario.<br>
<br>
To do that, we start by applying a big drop of solder to the solder pads to "fill" them. The solder needs to fully cover each solder pad we use, you can be generous here. Then, wire for wire, you reheat the "solder-drop" and gently push the tinned wire-ends into the melted solder drop and let it cool down to fix the connection. In the end, you only need to heat the heat shrink tubes for protection (if using). Voila, we've got ourselves a good and reliable connection.<br>
<br>
Next step SD module!
<br>
<br>

  #### 2.3 SD Module
For the SD card module the procedure is a bit different. Here we recommended a direct, non-detachable connection between SD module and PCB. Housing and PCB are designed in a way that the SD module is positioned directly under the PCB. This means we can use solid core wire, which does not need to be very flexible and saves the effort of tinning.<br>
<br>
Before you cut the wires to a length of ~40 mm, make sure that you remove any pre-installed pins from the SD module. The clean removal of pre-installed pins can be an art itself and you need to very careful here not to damage the rest of the module. This might be a good resource if you haven't done something like that before:
<br>
https://www.instructables.com/How-to-Desolder-Headers/
<br>
Then the process is a bit different. It makes sense to watch the video first before you go ahead, just to get the strategy. I recommend the following approach:<br>
<br>
Start and solder one end of each wire to the cleaned SD module.<br>

<br> -- IMAGE -- <br>

Then slip over some heat shrink and lead the SD module wires through the dedicated through holes on the PCB as if you were preparing to solder.<br>
<br>
After that, still keeping both components together, try to cautiously put down the PCB and SD module in the prepared housing elements. Make sure that everything is in its correct place, especially the SD module should nicely fit into the designated cavity.<br>

<br> -- IMAGE -- <br>

Now, for each through hole there should appear some remaining wire on the top side of the PCB. If this is the case, and everything is still in position, you can go ahead and solder. Afterwards, you should have some connecting wires of exactly the length you need. The last step is to use scissors or a side cutter to get rid of protruding wire on the top of PCB and apply the heat shrink below to protect the connection.<br>
<br>
<br>
<br>
  #### 2.4 RTC/Display module
Now it is time for the last component. Like we explained earlier, the component you use depends on the climateBOX version you want to build:<br>
<br>
V0.0 -> RTC<br>
<br>
V1.0 -> OLED display<br>
<br>
We can use either of these modules because they use the same PCB pins to communicate with the microcontroller.<br>
<br>
The process for both modules is roughly the same. However, we’d recommend the following: If building v0.0, directly solder the wired RTC to the bottom of the PCB. For v1.0 we use the proven combination of male header pins and female crimp connectors. We’ll quickly describe both alternatives below. But first, make sure to remove any previously installed header pins on the module you are going to use.<br>
<br>
**OLED display v1.0**
The preparation here is very similar to the revC sensor. You have to go through the process of cutting wires (AWG-22, length: ~ 100 mm), tinning (one end of each wire) and crimping (other end of the wire).<br>
<br>
After that, you can go ahead and solder the tinned ends of the wires to the OLED display module.<br>
<br>
Then push over some heat shrink if you like. Now, you can push the crimped connectors into the housing. Voila, you are done with the preparations and can assemble everything in **PART III**.<br>
<br>
<br>
**RTC v0.0**
The first steps for the RTC are similar to the ones above: cutting wires (AWG-22, length: ~ 100 mm) and tinning (both ends of each wire).<br>
<br>
Because we are going for a fixed solution here, where the RTC module is directly soldered to the PCB instead of using the detachable female connector, we save ourselves the crimping process. Just beware that a potential removal or exchange of this module later might be a bit of a tedious procedure.<br>
<br>
After tinning both wire ends, you can go ahead and solder one end of each wire to the RTC module, done! The rest will be covered in PART III.<br>
<br>
<br>
## 3. PART III: Preparing the housing
<br>
All housing parts can be 3D printed by using the .stl files in the *housing* folder.<br>
<br>
<br>
When printed, you should go ahead and check every part for potential printing errors and test-assemble everything. The explosion graphic below shows you how to do that. 
<br>
<br>
<img src='https://user-images.githubusercontent.com/49641232/215630392-d68c3e94-9fc1-4404-a4cd-8200f0625396.png' width="600" height="900">
<br>
<br>
In both, bottom and top element, there are notches. Later on, these notches accommodate a pair of screws that hold together the whole unit. We recommend to pre-drill the holes here. This will simplify the screwing process later.<br>
<br>
After you’ve done that, you can go ahead with the actual assembly.<br>
<br>
<br>
--------------------------------------------------
<br>
## 4. PART IV: Assembly
Now that you've prepared all parts and finished most of the work, it's time to put everything together. <br>
<br>
But before you proceed with the final assembly and put everything into the housing, it makes sense to test the climateBOX core and its connections (soldered/crimped) before. So, I recommend dividing the assembly into two steps:
<br>
-	core assembly
-	final assembly
<br>
  
  [PICTURE]
 
#### 4.1 Core assembly
First, solder all components which do not use crimped connectors and have not been attached to the PCB yet (e.g. RTC*, DHT*) to the board. If you have followed all steps until here, you should have done that already.<br>
<br>
For the components using crimped connectors (e.g. revC, DS18b20, OLED*), you can now go ahead and attach the prepared components via the male pins previously soldered to the PCB.<br>
<br>
A quick summary of the steps and my recommended connection in brackets:
-	attach DHT-22 to PCB (soldered OR female feather headers)
-	attach DS18b20 sensor to PCB (crimped connection using male pin headers)
-	attach revC sensor to PCB (crimped connection using male pin headers)
-	v0.0: attach RTC module to PCB (directly soldered to PCB)
-	v1.0: attach OLED display module to PCB (crimped connection using male pin headers)
-	make sure SD module is soldered to PCB and SD card is inserted
-	attach ESP32 (female feather headers)

  
#### 4.2 Connection test
  
#### 4.2.1 The Arduino IDE
Before you go ahead with the final assembly and put everything into the housing, it makes sense to test your core connections and make sure that everything works as expected. To do that, we need to upload the climateBOX program to the ESP32 board. This can be done using the Arduino IDE, a software that allows to write and send code to the microcontroller via USB. If haven't heard about Arduino, the web has a lot of fantastic resources to dig a little deeper. The Arduino IDE is briefly described here:<br>
   https://core-electronics.com.au/guides/arduino-ide-tutorial/<br>
<br>
So, depending on your operating system you should head to the official Arduino website and download the IDE for Windows or Mac.<br>
<br>
https://www.arduino.cc/en/software<br>

   
   
   
   #### 4.2.2 Adding the ESP32 board
   The software was originally written for Arduino boards, but it does also work with the ESP32. The only thing we need to do is to manually add the ESP32 board to the board library. The following source shows you how to do that:<br>
   https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/<br>
  

  
 #### 4.2.3 Required libraries
To use the sensors, you also need to install some custom libraries first. Again, here is a link that will show you how to do that:<br>
https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries<br>
<br>
If you are familiar with the process of installing additional libraries, please go ahead and install the following libs required to run the climateBOX<br>
<br>
a)
<br>
b)
<br>
c)
<br>

  #### 4.2.2 Test Connection
Now, that all libraries are installed, the last thing we need to do upload the climateBOX program. Again, here the program you have to use depends on the climateBOX version you want to build. The programs can be found in the *code* folder in this repository.<br>
<br>
If you haven’t uploaded a program to a microcontroller via the Arduino IDE yet, this link might be of help:<br>
https://support.arduino.cc/hc/en-us/articles/4733418441116-Upload-a-sketch-in-Arduino-IDE<br>
<br>
But before you upload the code to test your core assembly, it might be worth an excursion to the next section where I explain the program codes and their differences.<br>
<br>
When you have gone through the information in the next section, use the electrical circuit diagram to check all your connections once again and make sure you have connected all components in the right way.<br>
<br>
If you are confident that everything is in its correct spot, you can go ahead and connect your assembled PCB core to the computer via USB.<br>
<br>
Following the instructions in the provided link, make sure that you have selected the right board (DOIT ESP32 DEVKIT V1) and correct port.<br>
<br>
If everything is connected properly, the core device should work as expected and described in the program code section. You can now continue with the final assembly.<br>
<br>
Otherwise, if you are having problems, first check the following, common issues:<br>
Properly check all the connections for:<br>
<br>
-	Badly soldered connections
-	Loose connections, e.g. crimp connectors not properly attached to pins
-	Component attached in a wrong way
-	Wires attached in wrong order
-	SD not inserted
-	RTC is missing battery
<br>
<br>
If neither of those things fixes your problem, feel free to navigate to the discussion page and ask. 



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
  ## 5. Part V: The Program Code
  - Explanation climateBOX code
  - Highlight differences
  - Make sure that: USB wire is attached properly, SD card is inserted
  - connect climateBOX to PC/Mac via USB
  - Upload the climateBOX_rtc.ino file
