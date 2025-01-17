# streamer_item_extra_id

[![sampctl](https://img.shields.io/badge/sampctl-streamer_item_extra_id-2f2f2f.svg?style=for-the-badge)](https://github.com/Veseon/streamer_item_extra_id)

<!--
Short description of your library, why it's useful, some examples, pictures or
videos. Link to your forum release thread too.

Remember: You can use "forumfmt" to convert this readme to forum BBCode!

What the sections below should be used for:

`## Installation`: Leave this section un-edited unless you have some specific
additional installation procedure.

`## Testing`: Whether your library is tested with a simple `main()` and `print`,
unit-tested, or demonstrated via prompting the player to connect, you should
include some basic information for users to try out your code in some way.

And finally, maintaining your version number`:

* Follow [Semantic Versioning](https://semver.org/)
* When you release a new version, update `VERSION` and `git tag` it
* Versioning is important for sampctl to use the version control features

Happy Pawning!
-->

## Installation

Simply install to your project:

```bash
sampctl package install Veseon/streamer_item_extra_id
```

Include in your code and begin using the library:

```pawn
#include <streamer\item_extra_id>
```

## Usage

<!--
Write your code documentation or examples here. If your library is documented in
the source code, direct users there. If not, list your API and describe it well
in this section. If your library is passive and has no API, simply omit this
section.
-->

```pawn
bool:Streamer_SetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, EXTRA_ID_TYPE:type, value);
bool:Streamer_GetItemExtraId(STREAMER_TYPE:itemtype, STREAMER_ALL_TAGS:itemid, &EXTRA_ID_TYPE:type, &value);
bool:Streamer_GetPlayerExtraId(playerid, &EXTRA_ID_TYPE:type, &value);
```

## Testing

<!--
Depending on whether your package is tested via in-game "demo tests" or
y_testing unit-tests, you should indicate to readers what to expect below here.
-->

To test, simply run the package:

```bash
sampctl package run
```
