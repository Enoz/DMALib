#pragma once
namespace DMARender {
	struct IOverlay
	{
		virtual void DrawOverlay() = 0;
		virtual void createFonts() {};
	};
}


