#!c:\projects\zmq\zmqpyserver\.venv\scripts\python.exe
# EASY-INSTALL-ENTRY-SCRIPT: 'eggs==0.0.3','console_scripts','eggs'
__requires__ = 'eggs==0.0.3'
import re
import sys
from pkg_resources import load_entry_point

if __name__ == '__main__':
    sys.argv[0] = re.sub(r'(-script\.pyw?|\.exe)?$', '', sys.argv[0])
    sys.exit(
        load_entry_point('eggs==0.0.3', 'console_scripts', 'eggs')()
    )
