/* eos - A reimplementation of BioWare's Aurora engine
 * Copyright (c) 2010 Sven Hesse (DrMcCoy), Matthew Hoops (clone2727)
 *
 * The Infinity, Aurora, Odyssey and Eclipse engines, Copyright (c) BioWare corp.
 * The Electron engine, Copyright (c) Obsidian Entertainment and BioWare corp.
 *
 * This file is part of eos and is distributed under the terms of
 * the GNU General Public Licence. See COPYING for more informations.
 */

/** @file engines/nwn2/nwn2.cpp
 *  Engine class handling Neverwinter Nights 2
 */

#include "engines/nwn2/nwn2.h"

#include "common/util.h"
#include "common/strutil.h"
#include "common/filelist.h"
#include "common/stream.h"

#include "graphics/graphics.h"
#include "graphics/cube.h"
#include "graphics/font.h"
#include "graphics/text.h"

#include "sound/sound.h"

#include "events/events.h"

#include "aurora/resman.h"
#include "aurora/error.h"

namespace NWN2 {

const NWN2EngineProbe kNWN2EngineProbe;

const Common::UString NWN2EngineProbe::kGameName = "Neverwinter Nights 2";

Aurora::GameID NWN2EngineProbe::getGameID() const {
	return Aurora::kGameIDNWN2;
}

const Common::UString &NWN2EngineProbe::getGameName() const {
	return kGameName;
}

bool NWN2EngineProbe::probe(const Common::UString &directory, const Common::FileList &rootFiles) const {
	// If either the ini file or the binary is found, this should be a valid path
	if (rootFiles.contains(".*/nwn2.ini", true))
		return true;
	if (rootFiles.contains(".*/nwn2main.exe", true))
		return true;

	return false;
}

bool NWN2EngineProbe::probe(Common::SeekableReadStream &stream) const {
	return false;
}

Engines::Engine *NWN2EngineProbe::createEngine() const {
	return new NWN2Engine;
}


NWN2Engine::NWN2Engine() {
}

NWN2Engine::~NWN2Engine() {
}

void NWN2Engine::run(const Common::UString &target) {
	_baseDirectory = target;

	init();

	status("Successfully initialized the engine");

	playVideo("atarilogo");
	playVideo("oeilogo");
	playVideo("wotclogo");
	playVideo("nvidialogo");
	playVideo("legal");

	int channel = -1;

	Common::SeekableReadStream *wav = ResMan.getMusic("mus_mulsantir");
	if (wav) {
		// Cutting off the long silence at the end of mus_mulsantir :P
		wav = new Common::SeekableSubReadStream(wav, 0, 3545548, DisposeAfterUse::YES);

		status("Found a wav. Trying to play it. Turn up your speakers");
		channel = SoundMan.playSoundFile(wav, true);
	}

	Graphics::Cube *cube = 0;

	try {

		cube = new Graphics::Cube("wt_lake01_n");

	} catch (Common::Exception &e) {
		Common::printException(e);
	}

	while (!EventMan.quitRequested()) {
		EventMan.delay(10);
	}

	delete cube;
}

void NWN2Engine::init() {
	ResMan.registerDataBaseDir(_baseDirectory);

	ResMan.addArchiveDir(Aurora::kArchiveZIP, "data");

	status("Loading main resource files");

	indexMandatoryArchive(Aurora::kArchiveZIP, "2da.zip"           ,  0);
	indexMandatoryArchive(Aurora::kArchiveZIP, "actors.zip"        ,  1);
	indexMandatoryArchive(Aurora::kArchiveZIP, "animtags.zip"      ,  2);
	indexMandatoryArchive(Aurora::kArchiveZIP, "convo.zip"         ,  3);
	indexMandatoryArchive(Aurora::kArchiveZIP, "ini.zip"           ,  4);
	indexMandatoryArchive(Aurora::kArchiveZIP, "lod-merged.zip"    ,  5);
	indexMandatoryArchive(Aurora::kArchiveZIP, "music.zip"         ,  6);
	indexMandatoryArchive(Aurora::kArchiveZIP, "nwn2_materials.zip",  7);
	indexMandatoryArchive(Aurora::kArchiveZIP, "nwn2_models.zip"   ,  8);
	indexMandatoryArchive(Aurora::kArchiveZIP, "nwn2_vfx.zip"      ,  9);
	indexMandatoryArchive(Aurora::kArchiveZIP, "prefabs.zip"       , 10);
	indexMandatoryArchive(Aurora::kArchiveZIP, "scripts.zip"       , 11);
	indexMandatoryArchive(Aurora::kArchiveZIP, "sounds.zip"        , 12);
	indexMandatoryArchive(Aurora::kArchiveZIP, "soundsets.zip"     , 13);
	indexMandatoryArchive(Aurora::kArchiveZIP, "speedtree.zip"     , 14);
	indexMandatoryArchive(Aurora::kArchiveZIP, "templates.zip"     , 15);
	indexMandatoryArchive(Aurora::kArchiveZIP, "vo.zip"            , 16);
	indexMandatoryArchive(Aurora::kArchiveZIP, "walkmesh.zip"      , 17);

	status("Loading expansions resource files");

	// Expansion 1: Mask of the Betrayer (MotB)
	indexOptionalArchive(Aurora::kArchiveZIP, "2da_x1.zip"           , 20);
	indexOptionalArchive(Aurora::kArchiveZIP, "actors_x1.zip"        , 21);
	indexOptionalArchive(Aurora::kArchiveZIP, "animtags_x1.zip"      , 22);
	indexOptionalArchive(Aurora::kArchiveZIP, "convo_x1.zip"         , 23);
	indexOptionalArchive(Aurora::kArchiveZIP, "ini_x1.zip"           , 24);
	indexOptionalArchive(Aurora::kArchiveZIP, "lod-merged_x1.zip"    , 25);
	indexOptionalArchive(Aurora::kArchiveZIP, "music_x1.zip"         , 26);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_materials_x1.zip", 27);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_models_x1.zip"   , 28);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_models_x2.zip"   , 29);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_vfx_x1.zip"      , 30);
	indexOptionalArchive(Aurora::kArchiveZIP, "prefabs_x1.zip"       , 31);
	indexOptionalArchive(Aurora::kArchiveZIP, "scripts_x1.zip"       , 32);
	indexOptionalArchive(Aurora::kArchiveZIP, "soundsets_x1.zip"     , 33);
	indexOptionalArchive(Aurora::kArchiveZIP, "sounds_x1.zip"        , 34);
	indexOptionalArchive(Aurora::kArchiveZIP, "speedtree_x1.zip"     , 35);
	indexOptionalArchive(Aurora::kArchiveZIP, "templates_x1.zip"     , 36);
	indexOptionalArchive(Aurora::kArchiveZIP, "vo_x1.zip"            , 37);
	indexOptionalArchive(Aurora::kArchiveZIP, "walkmesh_x1.zip"      , 38);

	// Expansion 2: Storm of Zehir (SoZ)
	indexOptionalArchive(Aurora::kArchiveZIP, "2da_x2.zip"           , 40);
	indexOptionalArchive(Aurora::kArchiveZIP, "actors_x2.zip"        , 41);
	indexOptionalArchive(Aurora::kArchiveZIP, "animtags_x2.zip"      , 42);
	indexOptionalArchive(Aurora::kArchiveZIP, "lod-merged_x2.zip"    , 43);
	indexOptionalArchive(Aurora::kArchiveZIP, "music_x2.zip"         , 44);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_materials_x2.zip", 45);
	indexOptionalArchive(Aurora::kArchiveZIP, "nwn2_vfx_x2.zip"      , 46);
	indexOptionalArchive(Aurora::kArchiveZIP, "prefabs_x2.zip"       , 47);
	indexOptionalArchive(Aurora::kArchiveZIP, "scripts_x2.zip"       , 48);
	indexOptionalArchive(Aurora::kArchiveZIP, "soundsets_x2.zip"     , 49);
	indexOptionalArchive(Aurora::kArchiveZIP, "sounds_x2.zip"        , 50);
	indexOptionalArchive(Aurora::kArchiveZIP, "speedtree_x2.zip"     , 51);
	indexOptionalArchive(Aurora::kArchiveZIP, "templates_x2.zip"     , 52);
	indexOptionalArchive(Aurora::kArchiveZIP, "vo_x2.zip"            , 53);

	warning("TODO: Mysteries of Westgate (MoW) resource files");
	warning("TODO: Patch resource files");

	status("Loading secondary resources");
	ResMan.loadSecondaryResources(60);

	status("Loading override files");
	ResMan.loadOverrideFiles(70);
}

} // End of namespace NWN2
