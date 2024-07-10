/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
int mysystem(char *command) {
  int status = 0;
  
  // ここにプログラムを書く
  int pid;
  if(command==NULL) status=1;
  if((pid=fork())<0){
    status=-1;
  } else if(pid!=0){
    wait(&status);
  } else {
    execl("/bin/sh", "sh", "-c", command, NULL);
    exit(127);
  }
  return status;
}

/* 実行例

ここに実行例を書く
% make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c
 % ./mysysmain "ls -l README.md"
mysystem:
-rw-r--r--  1 imakitayuuma  staff  2795  7  4 09:45 README.md
retval = 00000000
system:
-rw-r--r--  1 imakitayuuma  staff  2795  7  4 09:45 README.md
retval = 00000000
 % ./mysysmain ls -l README.md  
使い方 : ./mysysmain コマンド文字列
 % ./mysysmain "cat a"          
mysystem:
cat: a: No such file or directory
retval = 00000100
system:
cat: a: No such file or directory
retval = 00000100
% ./mysysmain "aa README.md"
mysystem:
Usage: aa command <options>

Commands:

  archive                     archive the contents of a directory
  extract                     extract files from an archive
  list                        list the contents of an archive
  convert                     convert an archive into another
  manifest                    alias for 'archive -manifest'
  verify                      verify dir contents using manifest
  check-and-fix               check and fix dir contents using manifest
  append                      archive the contents of a directory, and append the output to an existing archive

Options:

  -v                          Increase verbosity level to stderr. Default is silent operation
  -h                          Show usage and quit
  -i input_file               Input file, default is stdin
  -o output_file              Output file, default is stdout
  -d dir                      Target directory for archive/append/extract, name is not included in the archive
                              default is current directory
  -subdir subdir              Target subdirectory under 'dir', name will be included in the archive
                              Default is empty
  -D dir_and_subdir           Defines both 'dir' (dirname) and 'subdir' (basename)
  -p                          Generate destination path (file/directory) automatically based on source path:
                              'aa archive -d foo -p' => 'aa archive -d foo -o foo.aar'
                              'aa manifest -d foo -p' => 'aa archive -d foo -o foo.manifest'
                              'aa extract -i foo.aar -p' => 'aa extract -i foo.aar -d foo'
  -a algorithm                Algorithm for archive compression, one of lzfse, lzma, zlib, lz4, lzbitmap, raw (no compression)
  -b block_size               Blocks size for archive compression, integer with an optional suffix b, k, m, g
                              Default compression options for archive: -a lzfse -b 4m
                              Default compression options for other commands: -a lzfse -b 1m
  -x                          Do not cross volume boundaries when enumerating files for archive
                              Default is to cross volume boundaries
  -t n_threads                Number of threads to run for compression/decompression,
                              Default 8 on this machine
  -wt n_threads               Number of writer threads to run (extract only),
                              Default is number of worker threads
  -enable-dedup (-no-enable-dedup)
                              If set, and SLC fields are present in the archive,
                              files with same data will be extracted as clones
                              Default is -no-enable-dedup
  -enable-holes (-no-enable-holes)
                              If set, and the filesystem supports it, detect and create
                              holes in files to store 0-filled segments
                              Default is -no-enable-holes
  -ignore-eperm (-no-ignore-eperm)
                              If set, ignore EPERM (operation not permitted) errors when setting files attributes
                              Default is -ignore-eperm
  -ignore-cpio-hlc (-no-ignore-cpio-hlc)
                              If set, ignore the NLINK and INO fields when parsing cpio archives, and do not attempt
                              to generate hard link cluster fields (HLC) from these fields
                              Default is -no-ignore-cpio-hlc
  -manifest                   Alias for the following options:
                              -exclude-field dat
                              -include-field sh2,siz,idx,idz
                              -a lzfse -b 1m
  -imanifest input_manifest   Manifest for the input archive, used to accelerate processing when only a subset
                              of entries are selected
  -omanifest output_manifest  Manifest for the output archive, generated along the main archive using the default manifest options
  -list-format format         Output format for list, one of text, json
                              Default is text

Filesystem compression:

  -afsc algorithm             Filesystem compression algorithm to use to extract files archived with the compression flag set,
                              one of zlib, lzvn, lzfse, lzbitmap, copy, default, none
  -afsc-none                  Do not use filesystem compression, same as '-afsc none'
  -afsc-all                   Try to compress all extracted files with filesystem compression.  Non-compressible files will be stored uncompressed.

Encryption:

  -keychain                   Use Keychain to load/store symmetric keys and passwords
  -password file              File containing encryption password
  -password-value password    Encryption password
  -password-gen               When encrypting, generate a new random password
  -key file                   File containing encryption key
  -key-value key              Encryption key, either hex:... or base64:...
  -key-gen                    When encrypting, generate a new symmetric key
  -recipient-pub file         Recipient public key for encryption
  -recipient-pub-value key    Recipient public key for encryption (hex or base64 X9.63 encoded)
  -recipient-priv file        Recipient private key for decryption
  -recipient-priv-value key   Recipient private key for decryption (hex or base64 X9.63 encoded)
  -sign-pub file              Sender signing public key for decryption
  -sign-pub-value key         Sender signing public key for decryption (hex or base64 X9.63 encoded)
  -sign-priv file             Sender signing private key for encryption
  -sign-priv-value key        Sender signing private key for encryption (hex or base64 X9.63 encoded)
  -padding padding_size       Padding size, units are b=1 byte, k=2^10 bytes, m=2^20 bytes, g=2^30 bytes,
                              default is defined for each profile. Can also be 'none' or 0 for no padding, or 'adaptive'
                              for adaptive Padmé padding size. Numerical values other than 0 must be >= 16.
  -checksum digest            Checksum algorithm, one of none, murmur64, sha256

  When archiving, the Apple Encrypted Archive profile is selected based on the encryption options:
  -password* select the scrypt encryption mode
  -key* select the 256-bit symmetric key encryption mode
  -recipient* select the 256-bit ECDHE encryption mode
  -sign* select the 256-bit ECDSA signature mode available with symmetric key and ECDHE encryption modes
  For the full range of Apple Encrypted Archive features, pipe the output of uncompressed 'aa' to 'aea'
  e.g.   aa archive -a raw <aa options> | aea encrypt <aea options>

Entry selection:

  -include-path path          Include entries having `path` as a prefix
  -exclude-path path          Exclude entries having `path` as a prefix
  -include-path-list file     File containing a list of paths to include with -include-path
                              (one per line, empty lines ignored)
  -exclude-path-list file     File containing a list of paths to exclude with -exclude-path
                              (one per line, empty lines ignored)
  -include-regex expr         Include entries matching `expr` (regex(3) syntax)
  -exclude-regex expr         Exclude entries matching `expr` (regex(3) syntax)
  -exclude-name name          Exclude entries with a path component equal to `name`
  -rename from to             Replace archive paths matching 'from' prefix with 'to',
                              and exclude non-matching paths

  All the entry selection options above can be specified multiple times
  Entry selection works as follows:
  1. If at least one -include-* option is given, keep paths matching at least one rule
  2. If at least one -exclude-* option is given, skip paths matching at least one rule
  3. If at least one -rename option is given, keep paths matching at least one of the 'from'
     prefixes, and replace the longest matching 'from' prefix with the corresponding 'to' prefix
  For archive, if -d dir -i path is given, the archive will only contain the
  single entry dir/path, and path will be used as archive entry name
  For archive, if only -i path is given, it will be interpreted as -d dirname(path) -i basename(path).

Archive fields:

  -include-field fields       Add `fields` to the field key set
  -exclude-field fields       Remove/Ignore `fields` from the field key set

  `fields` is a comma separated list of fields, listed below.
  These options can be specified multiple times

  typ                         entry type (always included)
  pat                         entry path (always included for filesystem objects)
  lnk                         link path (always included for symbolic links)
  dev                         device id (always included for block/character devices)
  uid                         user id
  gid                         group id
  mod                         access mode
  flg                         BSD flags
  mtm                         modification time
  btm                         backup time
  ctm                         creation time
  dat                         file data
  siz                         file data size
  cks                         file data digest, POSIX 1003.2-1992 32 bit CRC
  sh1                         file data SHA-1 digest
  sh2                         file data SHA-256 digest
  sh3                         file data SHA-384 digest
  sh5                         file data SHA-512 digest
  xat                         extended attributes
  acl                         access control list
  duz                         entry disk usage
  idx                         entry index in input archive
  idz                         entry size in input archive
  yec                         file data error correcting codes
  yaf                         list of archive fields (metadata entry)
  attr                        alias for uid,gid,mod,flg,mtm,btm,ctm
  all                         alias for all fields (exclude only)

Entry types:

  -include-type types         Include only entries matching `types`
  -exclude-type types         Include only entries not matching `types`

                              `types` is a string including one or more of the characters below

  b                           block special
  c                           character special
  d                           directory
  f                           regular file
  h                           hard link
  l                           symbolic link
  m                           metadata entry (not a filesystem object)
  p                           fifo
  s                           socket


ERROR: invalid command: README.md
retval = 00000100
system:
Usage: aa command <options>

Commands:

  archive                     archive the contents of a directory
  extract                     extract files from an archive
  list                        list the contents of an archive
  convert                     convert an archive into another
  manifest                    alias for 'archive -manifest'
  verify                      verify dir contents using manifest
  check-and-fix               check and fix dir contents using manifest
  append                      archive the contents of a directory, and append the output to an existing archive

Options:

  -v                          Increase verbosity level to stderr. Default is silent operation
  -h                          Show usage and quit
  -i input_file               Input file, default is stdin
  -o output_file              Output file, default is stdout
  -d dir                      Target directory for archive/append/extract, name is not included in the archive
                              default is current directory
  -subdir subdir              Target subdirectory under 'dir', name will be included in the archive
                              Default is empty
  -D dir_and_subdir           Defines both 'dir' (dirname) and 'subdir' (basename)
  -p                          Generate destination path (file/directory) automatically based on source path:
                              'aa archive -d foo -p' => 'aa archive -d foo -o foo.aar'
                              'aa manifest -d foo -p' => 'aa archive -d foo -o foo.manifest'
                              'aa extract -i foo.aar -p' => 'aa extract -i foo.aar -d foo'
  -a algorithm                Algorithm for archive compression, one of lzfse, lzma, zlib, lz4, lzbitmap, raw (no compression)
  -b block_size               Blocks size for archive compression, integer with an optional suffix b, k, m, g
                              Default compression options for archive: -a lzfse -b 4m
                              Default compression options for other commands: -a lzfse -b 1m
  -x                          Do not cross volume boundaries when enumerating files for archive
                              Default is to cross volume boundaries
  -t n_threads                Number of threads to run for compression/decompression,
                              Default 8 on this machine
  -wt n_threads               Number of writer threads to run (extract only),
                              Default is number of worker threads
  -enable-dedup (-no-enable-dedup)
                              If set, and SLC fields are present in the archive,
                              files with same data will be extracted as clones
                              Default is -no-enable-dedup
  -enable-holes (-no-enable-holes)
                              If set, and the filesystem supports it, detect and create
                              holes in files to store 0-filled segments
                              Default is -no-enable-holes
  -ignore-eperm (-no-ignore-eperm)
                              If set, ignore EPERM (operation not permitted) errors when setting files attributes
                              Default is -ignore-eperm
  -ignore-cpio-hlc (-no-ignore-cpio-hlc)
                              If set, ignore the NLINK and INO fields when parsing cpio archives, and do not attempt
                              to generate hard link cluster fields (HLC) from these fields
                              Default is -no-ignore-cpio-hlc
  -manifest                   Alias for the following options:
                              -exclude-field dat
                              -include-field sh2,siz,idx,idz
                              -a lzfse -b 1m
  -imanifest input_manifest   Manifest for the input archive, used to accelerate processing when only a subset
                              of entries are selected
  -omanifest output_manifest  Manifest for the output archive, generated along the main archive using the default manifest options
  -list-format format         Output format for list, one of text, json
                              Default is text

Filesystem compression:

  -afsc algorithm             Filesystem compression algorithm to use to extract files archived with the compression flag set,
                              one of zlib, lzvn, lzfse, lzbitmap, copy, default, none
  -afsc-none                  Do not use filesystem compression, same as '-afsc none'
  -afsc-all                   Try to compress all extracted files with filesystem compression.  Non-compressible files will be stored uncompressed.

Encryption:

  -keychain                   Use Keychain to load/store symmetric keys and passwords
  -password file              File containing encryption password
  -password-value password    Encryption password
  -password-gen               When encrypting, generate a new random password
  -key file                   File containing encryption key
  -key-value key              Encryption key, either hex:... or base64:...
  -key-gen                    When encrypting, generate a new symmetric key
  -recipient-pub file         Recipient public key for encryption
  -recipient-pub-value key    Recipient public key for encryption (hex or base64 X9.63 encoded)
  -recipient-priv file        Recipient private key for decryption
  -recipient-priv-value key   Recipient private key for decryption (hex or base64 X9.63 encoded)
  -sign-pub file              Sender signing public key for decryption
  -sign-pub-value key         Sender signing public key for decryption (hex or base64 X9.63 encoded)
  -sign-priv file             Sender signing private key for encryption
  -sign-priv-value key        Sender signing private key for encryption (hex or base64 X9.63 encoded)
  -padding padding_size       Padding size, units are b=1 byte, k=2^10 bytes, m=2^20 bytes, g=2^30 bytes,
                              default is defined for each profile. Can also be 'none' or 0 for no padding, or 'adaptive'
                              for adaptive Padmé padding size. Numerical values other than 0 must be >= 16.
  -checksum digest            Checksum algorithm, one of none, murmur64, sha256

  When archiving, the Apple Encrypted Archive profile is selected based on the encryption options:
  -password* select the scrypt encryption mode
  -key* select the 256-bit symmetric key encryption mode
  -recipient* select the 256-bit ECDHE encryption mode
  -sign* select the 256-bit ECDSA signature mode available with symmetric key and ECDHE encryption modes
  For the full range of Apple Encrypted Archive features, pipe the output of uncompressed 'aa' to 'aea'
  e.g.   aa archive -a raw <aa options> | aea encrypt <aea options>

Entry selection:

  -include-path path          Include entries having `path` as a prefix
  -exclude-path path          Exclude entries having `path` as a prefix
  -include-path-list file     File containing a list of paths to include with -include-path
                              (one per line, empty lines ignored)
  -exclude-path-list file     File containing a list of paths to exclude with -exclude-path
                              (one per line, empty lines ignored)
  -include-regex expr         Include entries matching `expr` (regex(3) syntax)
  -exclude-regex expr         Exclude entries matching `expr` (regex(3) syntax)
  -exclude-name name          Exclude entries with a path component equal to `name`
  -rename from to             Replace archive paths matching 'from' prefix with 'to',
                              and exclude non-matching paths

  All the entry selection options above can be specified multiple times
  Entry selection works as follows:
  1. If at least one -include-* option is given, keep paths matching at least one rule
  2. If at least one -exclude-* option is given, skip paths matching at least one rule
  3. If at least one -rename option is given, keep paths matching at least one of the 'from'
     prefixes, and replace the longest matching 'from' prefix with the corresponding 'to' prefix
  For archive, if -d dir -i path is given, the archive will only contain the
  single entry dir/path, and path will be used as archive entry name
  For archive, if only -i path is given, it will be interpreted as -d dirname(path) -i basename(path).

Archive fields:

  -include-field fields       Add `fields` to the field key set
  -exclude-field fields       Remove/Ignore `fields` from the field key set

  `fields` is a comma separated list of fields, listed below.
  These options can be specified multiple times

  typ                         entry type (always included)
  pat                         entry path (always included for filesystem objects)
  lnk                         link path (always included for symbolic links)
  dev                         device id (always included for block/character devices)
  uid                         user id
  gid                         group id
  mod                         access mode
  flg                         BSD flags
  mtm                         modification time
  btm                         backup time
  ctm                         creation time
  dat                         file data
  siz                         file data size
  cks                         file data digest, POSIX 1003.2-1992 32 bit CRC
  sh1                         file data SHA-1 digest
  sh2                         file data SHA-256 digest
  sh3                         file data SHA-384 digest
  sh5                         file data SHA-512 digest
  xat                         extended attributes
  acl                         access control list
  duz                         entry disk usage
  idx                         entry index in input archive
  idz                         entry size in input archive
  yec                         file data error correcting codes
  yaf                         list of archive fields (metadata entry)
  attr                        alias for uid,gid,mod,flg,mtm,btm,ctm
  all                         alias for all fields (exclude only)

Entry types:

  -include-type types         Include only entries matching `types`
  -exclude-type types         Include only entries not matching `types`

                              `types` is a string including one or more of the characters below

  b                           block special
  c                           character special
  d                           directory
  f                           regular file
  h                           hard link
  l                           symbolic link
  m                           metadata entry (not a filesystem object)
  p                           fifo
  s                           socket


ERROR: invalid command: README.md
retval = 00000100
% ./mysysmain "aabb README.md"
mysystem:
sh: aabb: command not found
retval = 00007f00
system:
sh: aabb: command not found
retval = 00007f00
*/
