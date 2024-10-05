Automatically inserts license information into PHP files within a specified directory.

Compile:
```bash
g++ -std=c++17 -o linsert linsert.cpp
```

Run:
```bash
./linsert src license.php
```
- ```src```: This is the path to the directory containing the PHP files where you want to insert the license. Adjust this to point to your actual source directory.
- ```license.php```: This is the path to the file containing the license information you wish to insert. Ensure that this file contains the license text formatted correctly.

License example:
```php
<?php
/**
 * License text goes here.
 */
```