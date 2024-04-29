#include "SmokeVoxel.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASmokeVoxel::ASmokeVoxel() {
	::ConstructorHelpers::FObjectFinder<UStaticMesh> smokeMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	m_pSmokeVoxelMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Smoke Voxel Mesh Component"));
	m_pSmokeVoxelMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (smokeMesh.Succeeded())
		m_pSmokeVoxelMeshComponent->SetStaticMesh(smokeMesh.Object);

	PrimaryActorTick.bCanEverTick = false;
}