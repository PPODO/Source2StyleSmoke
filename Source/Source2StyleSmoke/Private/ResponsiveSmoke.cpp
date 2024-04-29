#include "ResponsiveSmoke.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SmokeVoxel.h"
#include "DrawDebugHelpers.h"

AResponsiveSmoke::AResponsiveSmoke() {
	m_vListOfCheckAxes.Add(FVector(1.0, 0.0, 0.0));
	m_vListOfCheckAxes.Add(FVector(-1.0, 0.0, 0.0));
	m_vListOfCheckAxes.Add(FVector(0.0, 1.0, 0.0));
	m_vListOfCheckAxes.Add(FVector(0.0, -1.0, 0.0));
	m_vListOfCheckAxes.Add(FVector(0.0, 0.0, 1.0));
	m_vListOfCheckAxes.Add(FVector(0.0, 0.0, -1.0));

	m_vNextPositionQueue = std::make_unique<TArray<FVector>>();
	m_vCurrentCheckPositionQueue = std::make_unique<TArray<FVector>>();

	m_fElpasedDeltaTime = 0;
	m_iTotalEnergy = 0;
	m_fCellSize = 0.f;
	m_bIsExpansion = false;
	PrimaryActorTick.bCanEverTick = true;
}

void AResponsiveSmoke::GenerateSmoke() {
	m_vNextPositionQueue->Push(GetActorLocation());

	m_fCellSize = m_fTargetVolumeDiameter / m_iGridResolution;
	m_iTotalEnergy = (m_iGridResolution * m_iGridResolution * m_iGridResolution) - 1;
	m_bIsExpansion = true;
}

void AResponsiveSmoke::BeginPlay() {
	Super::BeginPlay();

}

void AResponsiveSmoke::Tick(float fDeltaTime) {
	Super::Tick(fDeltaTime);

	CalculateVoxel(fDeltaTime);
}

void AResponsiveSmoke::CalculateVoxel(float fDeltaTime) {
	m_fElpasedDeltaTime += fDeltaTime;

	if (m_bIsExpansion && m_fElpasedDeltaTime > m_fVoxelSpreadRate) {
		TArray<FVector> vDebugPositions;
		m_fElpasedDeltaTime = 0.f;

		if (m_vNextPositionQueue->Num() <= 0)
			m_bIsExpansion = false;

		FHitResult hitResult;

		m_vNextPositionQueue.swap(m_vCurrentCheckPositionQueue);

		while (m_vCurrentCheckPositionQueue->Num() > 0 && m_iTotalEnergy > 0) {
			FVector vCurrentQueryPosition = (*m_vCurrentCheckPositionQueue)[0];

			for (const auto& vAxes : m_vListOfCheckAxes) {
				const FVector vCurrentNeighborPosition = vCurrentQueryPosition + (vAxes * FVector(m_fCellSize));

				if (!m_vCheckedPositions.Contains(vCurrentNeighborPosition)) {
					UKismetSystemLibrary::LineTraceSingle(this, vCurrentQueryPosition, vCurrentNeighborPosition, TraceTypeQuery1, false, m_actorToIgnore, EDrawDebugTrace::None, hitResult, true);

					if (!hitResult.bBlockingHit) {
						m_vNextPositionQueue->Add(vCurrentNeighborPosition);
						m_vCheckedPositions.Add(vCurrentNeighborPosition);
						m_vAcceptedPositions.Add(vCurrentNeighborPosition);
						vDebugPositions.Add(vCurrentNeighborPosition);
						
						GetWorld()->SpawnActor<ASmokeVoxel>(ASmokeVoxel::StaticClass(), FTransform(FRotator::ZeroRotator, vCurrentNeighborPosition, FVector(m_fCellSize / 50.f)));

						m_iTotalEnergy--;
					}
				}
			}
			m_vCurrentCheckPositionQueue->RemoveAt(0, 1, true);
		}

		for (const auto& It : vDebugPositions)
			DrawDebugBox(GetWorld(), It, FVector(m_fCellSize), FColor::Red, false, 5.f);
	}
}