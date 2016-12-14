# Encrypted Key Value File System
In this project, we implemented SKVFS on top of fuse -- filesystem in user space. Using existing system calls to support the required features that make the SKVFS file system work.

The KVFS is different from conventional file systems in two ways:

First, to make the file system efficient for sharing among multiple clients, the file system uses a flat key-value structure as GoogleFS. There is no real directory supports. Every file, including directories, is translated into a 256-bit key. The file system maintains some data structure to map the key to a real location.

Second, to make the file system more secure, the file system uses MD5 to hash file names. As a result, anyone else cannot easily figure out what was the original filename, directory structure. Although MD5 can be brute-force attacked these days, it still take quite a while for an attacker to figure out. 

how to begin with KVFS:
To begin, you need to make sure your linux install contains the following:
1. Fuse (2.9.7). Download fuse 2.9.7 from https://github.com/libfuse/libfuse/releases/tag/fuse-2.9.7. Follow the instruction in their website and install the package.

2. OpenSSL library. We need this library for MD5 functions. In ubuntu, you can get this package installed by using the following command:
sudo apt-get install libssl-dev

3. mount KVFS: 
In KVFS root directory, use "./configure" to generate the Makefile fits your machine setup. Then, navigate to the src subdirectory, and type "make". You will see the executable "kvfs" generated.

4. Use the KVFS to create a user space file system. 
Create an home directory and a mounted directory.
You need to make sure that the mounted directory that you created gives you the full permissions (e.g. use chmod to change mode or chown to change ownership). 
Type "./kvfs /home_directory /mounted_directory" in terminal.
