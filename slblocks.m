function blkStruct = slblocks
% This function specifies that the library 'webcrane_library
% should appear in the Library Browser with the 
% name 'webcrane_library'

    Browser.Library = 'webcrane_library';
    % 'webcrane_library' is the name of the library

    Browser.Name = 'webcrane_library';
    % 'webcrane_library' is the library name that appears
    % in the Library Browser

    blkStruct.Browser = Browser;