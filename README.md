# Cache_Simulation

This is one of the three projects in the course BU MET CS472, which requires an understanding of cache in computer architecture and how it works.

### Goal:

The goal is to design and implement a software simulation of a cache memory subsystem. Note that any real cache is invisible to software. This will be a simulation of the way that a real cache works. The  "cache" will be a software structure/class  that will contain information similar (but on a smaller and more simplified scale) to what would be in a real cache.  The "main memory" will be a 2K array. We can make it an array of integers or shorts (16-bits), but because it is simulating a byte-addressable memory, we won't be putting any value larger than 0xFF (255 decimal or 11111111 binary) in it.



We are supposed to implement a direct-mapped, write-back cache. The block size will be 16 bytes and the number of slot is 16.



### Steps:

1. Initialization:

   We create arrays for main memory in size of 2048 and a structure for slot which contains valid bit, dirty bit, tag, slot number, block offset, data[16]. Then we initialize the main memory according to the meaning and create 16 blocks as the structure of slot that indicate cache.

2. Create split address function:

   Just as what we did in the first project (disassembler), whenever we get a new address, we disassemble it into block offset, tag and slot number and put them into a temporary array , respectively.

3. Read byte function:

   After the address is processed within splitAddress function, we judge if we already have the address in the cache. We have a cache hit only if the tag matches as well as the valid bit is true. If we have a cache miss, we shall (1) copy the entire block from MM into cache, (2) update tag, (3) set valid bit into true(means that this block is occupied).

4. Write back function:

   For this particular project, we only need to implement a write back process. As I noted above in read byte process, we need to disassemble each address and check if hit or not, respectively. If we have a miss, we shall first check if the slot in the cache has been updated(dirty bit = true), if it's been updated, update MM from the cache. This step shall be done because if not, this previous data will be lost (stored in cache instead of MM). Then we copy the entire block from main memory into cache and update the new data into cache.

5. Build user interface:

   To fulfill the exact purpose of the project, a proper UI is needed, which contains a main menu and the way to redirect after each instruction is finished.

### Problems and solutions:

1. P: The misunderstanding of the process of writing back

   When I first created the structure of slot, I included preAdd and preData to temporarily store the current address and data in cache because I thought the cache shall know which data has been previously updated. So I used the following statement:
    block[j].preAdd = writeAdd;
    block[j].preData = writeData;

   Then whenever it comes to writing to MM from cache:

   MM[block[j].preAdd] = block[j].preData;

   However, even if I was able to get the correct answer, it was a misunderstanding of how writing back works.



   S: It was not until I got the score of the project that I knew the problem. So I asked TA for help. She told me that I should have followed the following process:

   1. During the writing back process, if we have a cache miss while the dirty bit is true, it means the current data in cache has **not** been copied into MM. So we first need to copy the entire block from cache into MM, in which step, we need to calculate the address to write back to by using the tag from the cache and the slot number. Then copy the data from the slot in the cache to the correct location in the memory:
   2. After updating MM, we then copy the entire block from MM into cache based on the address we got.
   3. Set the new data into cache. 
  startAddress = (block[j].tag << 8) + (slotNumber << 4);

  for(int i = 0; i < 0x10; i++){
  ​	MM[startAddress + i] = block[j].data[i];
  }
   for(int i = 0; i < 0x10; i++){
  ​	startAddress = writeAdd & 0xFFFFFFF0;
  ​    block[j].data[i] = MM[startAddress + i];
  }
  block[j].data[block[j].blockOff] = writeData;


Generally, the writing back process is the one that based on reading byte process, except we need to check if we have a dirty bit. If so, we first need to update MM. 



### Conclusion:

This is literally the second project in BU MET CS 472. Compared to the first one (disassembler), it is a little logically complex and time consuming. I actually felt regret when I first misconstrue the process of writing back. Anyway, I've learned how cache performs in computer architecture deeply as well as improved my programming skills.