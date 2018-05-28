sr=$HOME/usrx
src=$HOME/srcx
bin=$usr/bin

# Create source and installation directories
mkdir $usr
mkdir $src

# Add usr to the path
export PATH="$bin:$PATH"

# Installing autotool
# link: http://askubuntu.com/questions/430706/installing-autotools-autoconf

# Downloading and installing autoconf
cd $src
wget http://ftp.gnu.org/gnu/autoconf/autoconf-2.69.tar.gz
tar xf autoconf-2.69.tar.gz
cd autoconf-2.69
sh configure --prefix=$usr
make install
hash autoconf

# Downloading and installing automake
cd $src
wget http://ftp.gnu.org/gnu/automake/automake-1.15.tar.gz
tar xf automake-1.15.tar.gz
cd automake-1.15
sh configure --prefix=$usr
make install
hash automake

# Downloading and installing libtool
cd $src
wget https://ftp.gnu.org/gnu/libtool/libtool-2.4.6.tar.xz
tar xf libtool-2.4.6.tar.xz
cd libtool-2.4.6
sh configure --prefix=$usr
make install
hash libtool

# OpenMPI and OpenSHMEM installation with UCX
# link: https://github.com/openucx/ucx/wiki/OpenMPI-and-OpenSHMEM-installation-with-UCX

# UCX installation
cd $src
git clone https://github.com/openucx/ucx.git ucx
cd ucx
./autogen.sh
mkdir build
cd build
../configure --prefix=$usr
make
make install

# OpenMPI and OpenSHMEM installation
git clone https://github.com/open-mpi/ompi.git
cd ompi
./autogen.pl
mkdir build
cd build
../configure --prefix=$usr --with-ucx=$usr
make
make install

export C_INCLUDE_PATH=$C_INCLUDE_PATH:$usr/include
export CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$usr/include

cd $src
mkdir test
cd test
echo $'#include "shmem.h"\nint main() {\nshmem_init();\nshmem_finalize();\nreturn 0;\n}\n' > test.c
shmemcc test.c