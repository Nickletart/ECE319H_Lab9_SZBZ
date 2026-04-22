# ECE319H_Lab9_SZBZ
Centipede game for MSPM0g3507
This is a Centipede-style game. The player controls a blaster at the bottom of the screen, can move within the player area, and fires bullets upward. Mushrooms sit on an 8x8 tile grid. Centipedes move horizontally across the field, go down one row when blocked, then reverse direction. Multiple centipedes can exist at once because a hit can split one into two.

Expected behavior:

A bullet travels straight upward. It can hit mushrooms and centipede segments. When a bullet hits a mushroom, the bullet disappears immediately. Mushrooms have multiple damage states and should visually progress through them on repeated hits. When a mushroom reaches its final damage state, it dies and disappears completely from the screen. No dead mushroom sprite should linger.

When a bullet hits a centipede segment, the bullet disappears immediately. A mushroom should be created at that location, but it must snap to the mushroom grid, not stay at an arbitrary pixel location where the segment happened to be. So the spawned mushroom should align exactly like any other mushroom tile.

Centipede hit behavior:

If the tail segment is hit, the centipede just gets one segment shorter.
If the head segment is hit, the next segment becomes the new head and the centipede gets one segment shorter.
If a middle segment is hit, the original centipede becomes the front half, and a new centipede is created from the back half.
No segments should be lost except the one that was shot.
Example: if a 12-segment centipede is hit at segment index 4, the result should be a 4-segment parent and a 7-segment child.
The new child centipede should start by turning downward, then continue normally.
The new child’s body should stay properly spaced behind its head. Its segments should not collapse onto the head.

Centipedes should move continuously while they exist. Their body segments should follow properly and remain spaced, not pile up. When a split happens, old graphics from the pre-split centipede should disappear correctly, and only the new valid centipede bodies should remain visible.

If all centipedes are gone, a brand new 12-segment centipede should spawn at the same starting location and state as the normal initial spawn.

Centipedes should collide with obstacles in front of them and turn downward when blocked. Obstacles include at least walls and mushrooms. There was also a later intention for centipede-on-centipede collision so centipedes treat other centipedes as obstacles too.

The player loses a life if touched by a centipede or if a centipede reaches the bottom. If lives reach zero, the game returns to the main menu. The HUD should show score and lives, and update correctly when those values change.

Sound behavior:

firing plays a shoot sound only if a bullet actually spawns
centipedes play a repeating step sound while any centipede is alive
hitting/killing enemies uses an enemy-hit/death sound
player death plays a death sound

Rendering expectations:

dead bullets should not linger on screen
dead mushrooms should not linger on screen
removed centipede segments should not linger on screen
newly spawned/split centipedes should render correctly immediately

The user prefers minimal, localized code changes rather than large refactors.
